/*
 * A079648  Number of primes between n^2 and n^3.
 * https://oeis.org/A079648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pi(n^3 - 1) - pi(n^2) for n > 1, a(0) = a(1) = 0
 * Range:     u8 n <= 12, u16 n <= 94, u32 n <= 110, u64 n <= 110,
 *            u128 n <= 110, big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses the supplied prime counting routine compute_pi(idx_t). */

#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    /* For n = 0 or 1 the interval is empty. */
    if (n <= 1) {
        return (seq_t)0;
    }

    /* Compute n^2 and n^3.  All arithmetic is done in idx_t to avoid
       overflow for the intended range of n (up to about 10 000). */
    idx_t nsq = n * n;         /* n^2 */
    idx_t ncb = nsq * n;       /* n^3 */

    /* Count primes <= n^3-1 and <= n^2.  compute_pi returns a value of
       type seq_t, which is compatible with the return type of a(). */
    seq_t pi_hi = compute_pi(ncb - 1);
    seq_t pi_lo = compute_pi(nsq);

    /* Result is the number of primes strictly between n^2 and n^3. */
    return pi_hi - pi_lo;
}
