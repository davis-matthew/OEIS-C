/*
 * A056778  Number of 3-element antichains on an unlabeled n-element set;
 *          equivalence classes of monotone Boolean functions of n variables
 *          with 3 mincuts under action of symmetric group S_n.
 * https://oeis.org/A056778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   linear recurrence from the generating function A(x) = x^3*(2 + x
 *            + 2*x^2 + 4*x^3 - x^5 - 2*x^6) / ((1 - x)^8*(1 + x)^2*(1 + x +
 *            x^2)^2) Denominator coefficients Q[k] (k = 0..14) and numerator
 *            coefficients P[n] (n = 0..9) are hard‑coded. The recurrence is
 *            a(n) = P[n] - Σ_{k=1}^{min(14,n)} Q[k] * a...
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 100, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const int Q[15] = {
    1,  /* Q[0] */
   -4,  /* Q[1] */
    4,  /* Q[2] */
    2,  /* Q[3] */
   -2,  /* Q[4] */
   -4,  /* Q[5] */
   -3,  /* Q[6] */
   12,  /* Q[7] */
   -3,  /* Q[8] */
   -4,  /* Q[9] */
   -2,  /* Q[10] */
    2,  /* Q[11] */
    4,  /* Q[12] */
   -4,  /* Q[13] */
    1   /* Q[14] */
};

static const int P[10] = {
    0, 0, 0,   /* n = 0,1,2 */
    2,         /* n = 3 */
    1,         /* n = 4 */
    2,         /* n = 5 */
    4,         /* n = 6 */
    0,         /* n = 7 */
   -1,         /* n = 8 */
   -2          /* n = 9 */
};

static seq_t *CACHE = NULL;
static size_t CACHE_SIZE = 0;

static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= CACHE_SIZE) return;
    seq_t *tmp = (seq_t *)realloc(CACHE, needed * sizeof(seq_t));
    if (!tmp) abort();
    for (size_t i = CACHE_SIZE; i < needed; ++i) tmp[i] = 0;
    CACHE = tmp;
    CACHE_SIZE = needed;
}

seq_t a(idx_t n)
{
    size_t old_size = CACHE_SIZE;
    ensure_cache(n);
    for (idx_t i = (idx_t)old_size; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= 14 && k <= i; ++k) {
            sum += (seq_t)Q[k] * CACHE[i - k];
        }
        seq_t coeff = 0;
        if (i < (idx_t)(sizeof(P)/sizeof(P[0]))) coeff = (seq_t)P[i];
        CACHE[i] = coeff - sum;
    }
    return CACHE[n];
}
