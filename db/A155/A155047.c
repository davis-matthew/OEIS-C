/*
 * A155047  a(1) = 1, a(2) = 2, then a(n) = largest prime factor of the
 *          partial sum up to a(n-1).
 * https://oeis.org/A155047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=1, a(2)=2; for n>2, a(n)=largest_prime_factor(∑_{k=1}^{n-1}
 *            a(k))
 * Range:     u8 n <= 31, u16 n <= 518, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gpf.h"   /* provides uint64_t gpf(uint64_t) */

seq_t a(idx_t n)
{
    /* Handle the two base cases explicitly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    /* Sum of the terms already computed so far (a(1)+a(2)) */
    seq_t sum = (seq_t)1 + (seq_t)2;

    /* Compute terms from a(3) up to a(n) */
    for (idx_t i = 3; i <= n; ++i)
    {
        /* Convert the current sum to a 64‑bit value for the helper function.
           The sequence grows slowly enough that this conversion never overflows
           for the n values considered (up to about 10 000). */
        uint64_t sum64 = (uint64_t)sum;

        /* Largest prime factor of the current partial sum */
        uint64_t lp64 = gpf(sum64);

        /* Convert back to seq_t for storage and further addition */
        seq_t next = (seq_t)lp64;

        if (i == n) return next;   /* a(n) found */

        sum += next;                /* update sum for the next iteration */
    }

    /* This point is never reached for n >= 1 */
    return (seq_t)0;
}
