/*
 * A123884  Expansion of phi(x) * phi(-x^6) / chi(-x^2) in powers of x where
 *          phi(), chi() are Ramanujan theta functions.
 * https://oeis.org/A123884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^n in q^(-1/12) * eta(q^2)^4 * eta(q^6)^2
 *            / (eta(q)^2 * eta(q^4) * eta(q^12)) Implementation uses the
 *            logarithmic derivative method: If F(q)=∏_{m≥1}(1−q^m)^{c_m} then
 *            log F(q)=∑_{n≥1} L_n q^n with L_n = −(1/n)∑_{d|n} d·c_d and the
 *            coefficients satisfy a(0)=1, a(n)= (1/n)∑_{...
 * Range:     u8 n <= 52, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* cached a(n) values */
static size_t cache_size = 0;        /* number of cached elements (>=1) */

static int *c_exp   = NULL;          /* c_m exponents */
static long long *S_sum = NULL;      /* Σ_{d|m} d·c_d */
static int *T_mult  = NULL;          /* −S_sum[m] */


/* Compute all required values up to index n (inclusive). */
static void ensure_computed(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_size) return;          /* already done */

    /* Allocate or grow the arrays */
    if (cache_size == 0) {
        cache   = (seq_t*)malloc(needed * sizeof(seq_t));
        c_exp   = (int*)malloc(needed * sizeof(int));
        S_sum   = (long long*)malloc(needed * sizeof(long long));
        T_mult  = (int*)malloc(needed * sizeof(int));
    } else {
        if (needed > cache_size) {
            cache   = (seq_t*)realloc(cache,   needed * sizeof(seq_t));
            c_exp   = (int*)realloc(c_exp,   needed * sizeof(int));
            S_sum   = (long long*)realloc(S_sum, needed * sizeof(long long));
            T_mult  = (int*)realloc(T_mult,  needed * sizeof(int));
        }
    }

    /* Compute c_exp for all indices up to needed-1 */
    for (size_t m = 1; m < needed; ++m) {
        int v = -2;
        if ((m & 1) == 0) v += 4;          /* 2 | m */
        if (m % 6 == 0)   v += 2;          /* 6 | m */
        if (m % 4 == 0)   v -= 1;          /* 4 | m */
        if (m % 12 == 0)  v -= 1;          /* 12 | m */
        c_exp[m] = v;
    }
    c_exp[0] = 0;   /* unused, safely defined */

    /* Compute S_sum for all indices up to needed-1 */
    for (size_t m = 0; m < needed; ++m) S_sum[m] = 0;          /* init */
    for (size_t d = 1; d < needed; ++d) {
        int cd = c_exp[d];
        for (size_t m = d; m < needed; m += d) {
            S_sum[m] += (long long)d * cd;
        }
    }

    /* Compute T_mult = -S_sum */
    T_mult[0] = 0;   /* unused */
    for (size_t m = 1; m < needed; ++m) T_mult[m] = -(int)S_sum[m];

    /* Compute a(n) values */
    cache[0] = (seq_t)1;    /* a(0) = 1 */
    for (size_t k = 1; k < needed; ++k) {
        seq_t sum = 0;
        for (size_t j = 1; j <= k; ++j) {
            sum += (seq_t)T_mult[j] * cache[k - j];
        }
        cache[k] = sum / (seq_t)k;
    }

    cache_size = needed;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    ensure_computed(n);
    return cache[(size_t)n];
}
