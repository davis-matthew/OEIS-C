/*
 * A069160  Number of primes p such that n^2 < p < n^2 + pi(n), where pi(n) is
 *          the number of primes less than n.
 * https://oeis.org/A069160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the number of primes p such that n^2 < p < n^2 + pi(n)
 * Range:     u8 n <= 2147, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_pi, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here pi(n) is the number of primes less than or equal to n. */
#include <stdbool.h>
#include "../utils/compute_pi.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Compute pi(n) (number of primes <= n). */
    seq_t pi_seq = compute_pi(n);
    idx_t pi = (idx_t)pi_seq;          /* pi(n) is small enough for idx_t */

    if (pi == 0) return 0;             /* no primes to check */

    /* Lower bound: n^2 */
    idx_t start = (idx_t)((seq_t)n * (seq_t)n);

    seq_t count = 0;
    /* Count primes strictly between n^2 and n^2 + pi(n) */
    for (idx_t p = start + 1; p < start + pi; ++p) {
        if (is_prime(p)) count++;
    }

    return count;
}
