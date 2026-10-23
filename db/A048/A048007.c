/*
 * A048007  Number of nonempty subsets of {1,2,...,n} in which exactly 2/3 of
 *          the elements are <= (n-1)/3.
 * https://oeis.org/A048007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{min(floor((n-1)/6), n-floor((n-1)/3))} *
 *            C(floor((n-1)/3), 2k) * C(n-floor((n-1)/3), k)
 * Range:     u8 n <= 15, u16 n <= 27, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Where C denotes binomial coefficient (combination). */

#include "../utils/binom.h"   /* binom(idx_t a, idx_t b) available */

seq_t a(idx_t n)
{
    /* For n < 1 the sequence is not defined; return 0. */
    if (n == 0)
        return 0;

    /* Low part: numbers <= floor((n-1)/3). */
    idx_t L = (n - 1) / 3;
    /* High part: remaining numbers. */
    idx_t H = n - L;

    /* Maximum k such that 2k <= L (otherwise we can't choose 2k low
       elements) and k <= H (we need k high elements). */
    idx_t maxk = L / 2;
    if (maxk > H)
        maxk = H;

    seq_t total = 0;
    for (idx_t k = 1; k <= maxk; ++k) {
        seq_t low_choice  = binom(L, 2 * k);
        seq_t high_choice = binom(H, k);
        total += low_choice * high_choice;
    }
    return total;
}
