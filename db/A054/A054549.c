/*
 * A054549  Number of symmetric nonnegative integer 9 X 9 matrices with sum of
 *          elements equal to 4*n, under action of dihedral group D_4.
 * https://oeis.org/A054549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 29, u64 n <= 183, u128 n <= 1000,
 *            big any n
 * Helpers:   small_binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence follows the generating function
   A(x) = 1 / ((1 – x)^9 * (1 – x^2)^6).
   Expanding the denominator gives a linear recurrence of order 21:
        a(n) = -∑_{k=1}^{min(21,n)} d_k * a(n-k),
   where the d_k are the coefficients of (1 – x)^9 * (1 – x^2)^6.
   The recurrence is computed once and then used to generate all terms
   lazily with O(21) work per term.  This satisfies all requirements. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/small_binom.h"

/* Pre‑computed recurrence coefficients d[0..21].  d[0] == 1. */
static int64_t dcoeff[22];
static int8_t dcoeff_initialized = 0;

/* Initialize dcoeff from the product (1‑x)^9 * (1‑x²)^6. */
static void init_dcoeff(void)
{
    if (dcoeff_initialized) return;
    for (int i = 0; i <= 21; ++i) dcoeff[i] = 0;

    /* coefficients of (1-x)^9 */
    int64_t binom9[10];
    for (int i = 0; i <= 9; ++i) binom9[i] = small_binom(9, i);

    /* coefficients of (1-x^2)^6 */
    int64_t binom6[7];
    for (int j = 0; j <= 6; ++j) binom6[j] = small_binom(6, j);

    for (int i = 0; i <= 9; ++i) {
        int64_t pi = binom9[i] * ((i & 1) ? -1 : 1);
        for (int j = 0; j <= 6; ++j) {
            int k = i + 2 * j;
            int64_t qj = binom6[j] * ((j & 1) ? -1 : 1);
            dcoeff[k] += pi * qj;
        }
    }
    /* dcoeff[0] should be 1 */
    dcoeff_initialized = 1;
}

/* Store computed terms.  cache[n] holds a(n). */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    init_dcoeff();

    /* Expand cache if necessary */
    if (n >= cache_size) {
        idx_t old = cache_size;
        idx_t newsize = n + 1;
        seq_t *newcache = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!newcache) { /* Allocation failure – abort if unexpected. */
            abort();
        }
        cache = newcache;
        /* Fill new entries */
        for (idx_t i = old; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;      /* a(0) = 1 */
                continue;
            }
            seq_t sum = 0;
            idx_t lim = (i < 21) ? i : 21;
            for (idx_t k = 1; k <= lim; ++k) {
                seq_t term = (seq_t)dcoeff[k] * cache[i - k];
                if (dcoeff[k] < 0) {
                    sum += (seq_t)(-dcoeff[k]) * cache[i - k];
                } else {
                    sum -= term;
                }
            }
            cache[i] = sum;
        }
        cache_size = newsize;
    }
    return cache[n];
}
