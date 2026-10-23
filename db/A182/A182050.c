/*
 * A182050  Number of odd primes between successive numbers of the form
 *          n*(n+1)^2/2.
 * https://oeis.org/A182050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of odd primes strictly between * n*(n-1)^2/2 and
 *            n*(n+1)^2/2 * This is equivalent to pi(right-1) - pi(left), *
 *            where pi(x) is the prime-counting function
 * Range:     u8 n <= 7, u16 n <= 50, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    /* Compute the lower limit L = (n-1)*n^2/2
       and the upper limit R = n*(n+1)^2/2. */
    seq_t L = ((seq_t)(n - 1)) * (seq_t)n * (seq_t)n / 2;
    seq_t R = (seq_t)n * ((seq_t)n + 1) * ((seq_t)n + 1) / 2;

    /* Count primes <= R-1 minus primes <= L.
       Since R >= 2, R-1 is at least 1. */
    seq_t cnt_right = compute_pi((idx_t)(R - 1));
    seq_t cnt_left  = compute_pi((idx_t)L);

    return cnt_right - cnt_left;
}
