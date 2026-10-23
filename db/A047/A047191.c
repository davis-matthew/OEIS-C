/*
 * A047191  Number of nonempty subsets of {1,2,...,n} in which exactly 1/6 of
 *          the elements are <= (n-2)/2.
 * https://oeis.org/A047191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{min(⌊(n‑2)/2⌋, ⌊(n‑⌊(n‑2)/2⌋)/5⌋)} C(⌊(n‑2)/2⌋,
 *            k) · C(n‑⌊(n‑2)/2⌋, 5k)
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    /* use symmetry to reduce multiplicative steps */
    if (k > n - k) k = n - k;

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* multiply then divide; exact division holds */
        res = res * (n - k + i) / i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Compute ⌊(n‑2)/2⌋ */
    idx_t a_small = (n >= 2) ? (n - 2) / 2 : 0;
    idx_t a_big   = n - a_small;             /* elements > a_small */

    idx_t k_limit_small = a_small;
    idx_t k_limit_big   = a_big / 5;
    idx_t k_limit = (k_limit_small < k_limit_big) ? k_limit_small : k_limit_big;

    seq_t sum = 0;
    for (idx_t k = 1; k <= k_limit; ++k) {
        seq_t c1 = binom(a_small, k);
        seq_t c2 = binom(a_big, 5 * k);
        sum += c1 * c2;
    }
    return sum;
}
