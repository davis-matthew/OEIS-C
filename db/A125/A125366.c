/*
 * A125366  Number of base 29 circular n-digit numbers with adjacent digits
 *          differing by 4 or less.
 * https://oeis.org/A125366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace( M^n ), where M[i][j] = 1 if |i-j| ≤ 4 and 0
 *            otherwise
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* for bool, false, true */

#define BASE_N 29

/* ------------------------------------------------------------------ */
/* Precomputed adjacency matrix for base 29 with step ≤ 4.           */
/* The static flag guarantees it is constructed only once.          */
/* ------------------------------------------------------------------ */
static seq_t base_adj[BASE_N][BASE_N];
static bool base_initialized = false;

static void init_base_matrix(void)
{
    if (base_initialized)
        return;

    for (idx_t i = 0; i < BASE_N; ++i) {
        for (idx_t j = 0; j < BASE_N; ++j) {
            seq_t diff = (i >= j) ? (i - j) : (j - i);
            base_adj[i][j] = (diff <= 4U) ? (seq_t)1 : (seq_t)0;
        }
    }
    base_initialized = true;
}

/* ------------------------------------------------------------------ */
/* Matrix multiplication: r = a * b   (all 29×29 matrices).          */
/* ------------------------------------------------------------------ */
static void mat_mul(const seq_t a[BASE_N][BASE_N],
                    const seq_t b[BASE_N][BASE_N],
                    seq_t r[BASE_N][BASE_N])
{
    for (idx_t i = 0; i < BASE_N; ++i) {
        for (idx_t j = 0; j < BASE_N; ++j) {
            seq_t s = 0;
            for (idx_t k = 0; k < BASE_N; ++k) {
                s += a[i][k] * b[k][j];
            }
            r[i][j] = s;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Main sequence function.                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;   /* a(0) = 1 */

    init_base_matrix();

    /* res = identity matrix */
    seq_t res[BASE_N][BASE_N];
    for (idx_t i = 0; i < BASE_N; ++i) {
        for (idx_t j = 0; j < BASE_N; ++j)
            res[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
    }

    /* power = base_adj */
    seq_t power[BASE_N][BASE_N];
    for (idx_t i = 0; i < BASE_N; ++i)
        for (idx_t j = 0; j < BASE_N; ++j)
            power[i][j] = base_adj[i][j];

    idx_t exp = n;
    seq_t temp[BASE_N][BASE_N];

    while (exp > 0) {
        if (exp & (idx_t)1) {
            mat_mul(res, power, temp);
            for (idx_t i = 0; i < BASE_N; ++i)
                for (idx_t j = 0; j < BASE_N; ++j)
                    res[i][j] = temp[i][j];
        }
        mat_mul(power, power, temp);
        for (idx_t i = 0; i < BASE_N; ++i)
            for (idx_t j = 0; j < BASE_N; ++j)
                power[i][j] = temp[i][j];
        exp >>= 1;
    }

    /* trace of res */
    seq_t trace = 0;
    for (idx_t i = 0; i < BASE_N; ++i)
        trace += res[i][i];

    return trace;
}
