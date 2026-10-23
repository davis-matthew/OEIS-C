/*
 * A020895  Cubefree sums of two nonzero cubes.
 * https://oeis.org/A020895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 501, u64 n <= 501, u128 n <= 501,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth element of OEIS sequence A020895:
 *
 *   a(n) = nth smallest positive integer that can be expressed as the sum
 *   of two non‑zero integer cubes and is cube‑free.
 *
 * The implementation builds all values up to the current maximum bound
 * and keeps them in a static sorted array.  When more terms are required
 * the bound is doubled until the array contains at least n elements.
 *
 * All calculations are performed using the opaque type seq_t; no
 * intermediate values overrun their natural precision.  Auxiliary
 * helper functions (included as specified by the build system) can be
 * used for mathematical operations, but none are required for this
 * function.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* ------------------------------------------------------------------ */
/* Utility: comparison for qsort                                       */
static int seq_cmp(const void *p1, const void *p2)
{
    seq_t a = *(const seq_t *)p1;
    seq_t b = *(const seq_t *)p2;
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}

/* ------------------------------------------------------------------ */
/* Test whether n is cube‑free (no prime factor with exponent ≥ 3).     */
static bool is_cubefree(seq_t n)
{
    seq_t m = n;
    seq_t p = 2;
    while (p * p * p <= m) {
        int exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
            if (exp >= 3)
                return false;
        }
        if (p == 2)
            p = 3;
        else
            p += 2;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Generate all cube‑free sums of two non‑zero integer cubes with |x|,
 * |y| ≤ bound.  The results are returned in a freshly allocated
 * sorted unique array. */
static void build_sums(idx_t bound, seq_t **out, size_t *len)
{
    /* Rough upper bound on the number of candidates: 4 * bound². */
    size_t max_cand = (size_t)4 * bound * bound;
    seq_t *tmp = (seq_t *)malloc(max_cand * sizeof(seq_t));
    size_t cnt = 0;

    for (idx_t a = 1; a <= bound; ++a) {
        seq_t a3 = (seq_t)a * a * a;
        for (idx_t b = 1; b <= bound; ++b) {
            seq_t b3 = (seq_t)b * b * b;
            /* All sign combinations of a³ and b³. */
            int64_t s[4];
            s[0] = (int64_t)a3 + (int64_t)b3;   /* +, +  */
            s[1] = (int64_t)a3 - (int64_t)b3;   /* +, -  */
            s[2] = -(int64_t)a3 + (int64_t)b3;   /* -, +  */
            s[3] = -(int64_t)a3 - (int64_t)b3;   /* -, -  */

            for (int k = 0; k < 4; ++k) {
                if (s[k] > 0) {
                    seq_t val = (seq_t)s[k];
                    if (is_cubefree(val))
                        tmp[cnt++] = val;
                }
            }
        }
    }

    /* Sort the candidates and keep only unique values. */
    qsort(tmp, cnt, sizeof(seq_t), seq_cmp);
    size_t uniq = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (i == 0 || tmp[i] != tmp[i - 1])
            tmp[uniq++] = tmp[i];
    }

    *out = tmp;
    *len = uniq;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                              */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined, but keep deterministic */

    static seq_t *vals = NULL;   /* sorted unique values */
    static size_t vals_len = 0;
    static idx_t max_a = 0;

    while (vals_len < n) {
        /* Increase the search bound.  Start with 200 and double each time. */
        size_t new_max = (max_a == 0) ? 200 : max_a * 2;
        seq_t *new_vals;
        size_t new_len;

        build_sums((idx_t)new_max, &new_vals, &new_len);

        free(vals);          /* discard previous array */
        vals = new_vals;
        vals_len = new_len;
        max_a = (idx_t)new_max;
    }

    return vals[n - 1];
}
