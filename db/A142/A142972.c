/*
 * A142972  a(n) = the largest integer such that each positive integer <= a(n)
 *          divides at least one integer k, where the n-th prime <= k <= the
 *          (n+1)th prime.
 * https://oeis.org/A142972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the largest integer such that each positive integer <=
 *            a(n) divides at least one integer k in the closed interval [p_n,
 *            p_{n+1}], where p_n is the n-th prime. (OEIS A142972)
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    /* n >= 1 as per OEIS offset; behaviour for n == 0 is irrelevant */
    idx_t p1 = (idx_t)get_prime(n);        /* nth prime */
    idx_t p2 = (idx_t)get_prime(n + 1);    /* (n+1)-th prime */

    /* All integers i <= L (the prime gap) automatically have a multiple
       in the interval, so we can start testing from i = L+1. */
    idx_t L = p2 - p1;                     /* prime gap */

    /* Search for the smallest i > L such that no multiple of i lies in
       [p1, p2].  The first such i gives a(n) = i - 1. */
    for (idx_t i = L + 1; ; ++i) {
        /* Count multiples of i in the interval. */
        idx_t cnt = p2 / i - (p1 - 1) / i;
        if (cnt == 0) {
            return (seq_t)(i - 1);
        }
    }
}
