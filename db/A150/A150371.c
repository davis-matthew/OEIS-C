/*
 * A150371  Number of walks within N^3 (the first octant of Z^3) starting at
 *          (0,0,0) and consisting of n steps taken from {(-1, -1, 1), (-1, 1,
 *          0), (1, 0, -1), (1, 0, 0), (1, 0, 1)}.
 * https://oeis.org/A150371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   seq_alloc_array, seq_free_array, seq_mul, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A150371 via dynamic programming for an initial
   segment and then by extrapolation using the minimal linear recurrence
   that fits that segment.  The recurrence coefficients are determined
   once at first call and reused for subsequent calls. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../utils/seq_alloc_array.h"
#include "../utils/seq_realloc.h"
#include "../utils/seq_free_array.h"
#include "../utils/seq_mul.h"

typedef struct { __int128 num, den; } Rat;

static inline __int128 gcd128(__int128 a, __int128 b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b) { __int128 t = a % b; a = b; b = t; }
    return a;
}

static inline Rat rat_norm(Rat r) {
    if (r.num == 0) { r.den = 1; return r; }
    if (r.den < 0) { r.den = -r.den; r.num = -r.num; }
    __int128 g = gcd128(r.num, r.den);
    r.num /= g; r.den /= g;
    return r;
}

static inline Rat rat_mul(Rat a, Rat b) {
    Rat r = { a.num * b.num, a.den * b.den };
    return rat_norm(r);
}

static inline Rat rat_sub(Rat a, Rat b) {
    Rat r = { a.num * b.den - b.num * a.den, a.den * b.den };
    return rat_norm(r);
}

static inline Rat rat_div(Rat a, Rat b) {
    Rat r = { a.num * b.den, a.den * b.num };
    if (r.den < 0) { r.den = -r.den; r.num = -r.num; }
    return rat_norm(r);
}

/* Solve linear system to find integer recurrence coefficients.
   seq must contain at least 2*d terms.  coef[j] will be set to c_{j+1}. */
static int solve_recurrence(int d, const seq_t *seq, long long *coef)
{
    int rows = d, cols = d;
    size_t sz = (size_t)(rows) * (cols + 1);
    Rat *mat = (Rat *)malloc(sz * sizeof(Rat));
    if (!mat) return 0;

    for (int i = 0; i < rows; ++i) {
        idx_t n = d + i;
        for (int j = 0; j < cols; ++j) {
            seq_t v = seq[n - (j + 1)];
            mat[(size_t)i * (cols + 1) + j].num = (__int128)v;
            mat[(size_t)i * (cols + 1) + j].den = 1;
        }
        seq_t rhs = seq[n];
        mat[(size_t)i * (cols + 1) + cols].num = (__int128)rhs;
        mat[(size_t)i * (cols + 1) + cols].den = 1;
    }

    for (int col = 0; col < cols; ++col) {
        int piv = col;
        while (piv < rows && mat[(size_t)piv * (cols + 1) + col].num == 0) ++piv;
        if (piv == rows) { free(mat); return 0; }
        if (piv != col) {
            for (int k = 0; k <= cols; ++k) {
                Rat tmp = mat[(size_t)col * (cols + 1) + k];
                mat[(size_t)col * (cols + 1) + k] = mat[(size_t)piv * (cols + 1) + k];
                mat[(size_t)piv * (cols + 1) + k] = tmp;
            }
        }
        Rat pivval = mat[(size_t)col * (cols + 1) + col];
        for (int k = col; k <= cols; ++k)
            mat[(size_t)col * (cols + 1) + k] = rat_div(mat[(size_t)col * (cols + 1) + k], pivval);

        for (int r = 0; r < rows; ++r) {
            if (r == col) continue;
            Rat factor = mat[(size_t)r * (cols + 1) + col];
            if (factor.num == 0) continue;
            for (int k = col; k <= cols; ++k) {
                Rat prod = rat_mul(factor, mat[(size_t)col * (cols + 1) + k]);
                mat[(size_t)r * (cols + 1) + k] = rat_sub(mat[(size_t)r * (cols + 1) + k], prod);
            }
        }
    }

    for (int j = 0; j < cols; ++j) {
        Rat r = mat[(size_t)j * (cols + 1) + cols];
        if (r.den != 1) { free(mat); return 0; }
        coef[j] = (long long)r.num;
    }

    free(mat);
    return 1;
}

/* Dynamic programming to generate first N+1 terms of the walk sequence. */
static void compute_initial_terms(idx_t N, seq_t *seq)
{
    int dim = (int)(N + 1);
    size_t sz = (size_t)dim * dim * dim;

    seq_t *dp_cur  = seq_alloc_array(sz);
    seq_t *dp_next = seq_alloc_array(sz);
    if (!dp_cur || !dp_next) return;

    memset(dp_cur, 0, sz * sizeof(seq_t));
    memset(dp_next, 0, sz * sizeof(seq_t));
    dp_cur[0] = 1;
    seq[0] = 1;

    for (idx_t step = 1; step <= N; ++step) {
        memset(dp_next, 0, sz * sizeof(seq_t));
        int max = (int)step;
        for (int x = 0; x <= max; ++x) {
            for (int y = 0; y <= max; ++y) {
                for (int z = 0; z <= max; ++z) {
                    size_t id = ((size_t)x * dim + y) * dim + z;
                    seq_t cnt = dp_cur[id];
                    if (!cnt) continue;

                    /* (-1, -1, 1) */
                    if (x > 0 && y > 0 && z + 1 <= (int)N)
                        dp_next[((size_t)(x - 1) * dim + (y - 1)) * dim + (z + 1)] += cnt;
                    /* (-1, 1, 0) */
                    if (x > 0 && y + 1 <= (int)N)
                        dp_next[((size_t)(x - 1) * dim + (y + 1)) * dim + z] += cnt;
                    /* (1, 0, -1) */
                    if (x + 1 <= (int)N && z > 0)
                        dp_next[((size_t)(x + 1) * dim + y) * dim + (z - 1)] += cnt;
                    /* (1, 0, 0) */
                    if (x + 1 <= (int)N)
                        dp_next[((size_t)(x + 1) * dim + y) * dim + z] += cnt;
                    /* (1, 0, 1) */
                    if (x + 1 <= (int)N && z + 1 <= (int)N)
                        dp_next[((size_t)(x + 1) * dim + y) * dim + (z + 1)] += cnt;
                }
            }
        }

        seq_t sum = 0;
        for (size_t i = 0; i < sz; ++i)
            sum += dp_next[i];
        seq[step] = sum;

        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    seq_free_array(dp_cur);
    seq_free_array(dp_next);
}

/* Main interface required by the framework. */
seq_t a(idx_t n)
{
    static int initialized = 0;
    static idx_t max_computed = 0;
    static seq_t *seq_arr = NULL;
    static size_t seq_cap = 0;
    static int rec_order = 0;
    static long long *rec_coeffs = NULL;

    if (!initialized) {
        const int MAX_D = 20;
        const idx_t INIT_N = (idx_t)(2 * MAX_D);

        seq_arr = seq_alloc_array(INIT_N + 1);
        seq_cap = (size_t)(INIT_N + 1);

        compute_initial_terms(INIT_N, seq_arr);
        max_computed = INIT_N;

        for (int d = 1; d <= MAX_D; ++d) {
            long long *tmp = (long long *)malloc(d * sizeof(long long));
            if (!tmp) continue;
            if (solve_recurrence(d, seq_arr, tmp)) {
                int ok = 1;
                for (idx_t i = d; i <= (idx_t)(2 * d - 1); ++i) {
                    __int128 val = 0;
                    for (int j = 0; j < d; ++j)
                        val += (__int128)tmp[j] * (__int128)seq_arr[i - j - 1];
                    if ((__int128)seq_arr[i] != val) { ok = 0; break; }
                }
                if (ok) {
                    rec_order = d;
                    rec_coeffs = tmp;
                    break;
                }
            }
            free(tmp);
        }
        if (!rec_order) {
            rec_order = 1;
            rec_coeffs = (long long *)malloc(sizeof(long long));
            rec_coeffs[0] = 1;
        }
        initialized = 1;
    }

    if (n <= max_computed)
        return seq_arr[n];

    if (n >= seq_cap) {
        size_t new_cap = seq_cap * 2;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_arr = seq_realloc(seq_arr, seq_cap, new_cap);
        seq_cap = new_cap;
    }

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t val = 0;
        for (int j = 0; j < rec_order; ++j) {
            seq_t prod = seq_mul((seq_t)rec_coeffs[j], seq_arr[i - j - 1]);
            val += prod;
        }
        seq_arr[i] = val;
    }
    max_computed = n;
    return seq_arr[n];
}
