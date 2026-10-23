/*
 * A048015  Number of nonempty subsets of {1,2,...,n} in which exactly 5/6 of
 *          the elements are <= (n-1)/3.
 * https://oeis.org/A048015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{min(floor(t/5),(n-t))} C(t,5k)*C(n-t,k), where
 *            t = floor((n-1)/3)
 * Range:     u8 n <= 21, u16 n <= 39, u32 n <= 69, u64 n <= 129,
 *            u128 n <= 246, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static inline seq_t binom_idx(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        result = result * (seq_t)(n - k + i);
        result = result / (seq_t)i;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* out of defined range */
    idx_t t = (n - 1) / 3;          /* floor((n-1)/3) */
    idx_t bottom = n - t;
    idx_t kmax = t / 5;
    if (kmax > bottom) kmax = bottom;
    if (kmax == 0) return 0;

    seq_t sum = 0;
    for (idx_t k = 1; k <= kmax; ++k) {
        seq_t c1 = binom_idx(t, 5 * k);
        seq_t c2 = binom_idx(bottom, k);
        sum += c1 * c2;
    }
    return sum;
}
