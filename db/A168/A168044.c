/*
 * A168044  Half of the even nonisolated nonprimes A167692.
 * https://oeis.org/A168044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = index of numbers n such that at least one of 2n-1 and
 *            2n+1 is composite (with the first term a(1) = 0)
 * Range:     u8 n <= 122, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* The first term is always 0. */
    if (n == 1)
        return (seq_t)0;

    /* Count the terms that have been found so far.
       Initially we have already counted 0 as the first term. */
    idx_t count = 1;
    /* Candidate value for n.  We start checking from 1. */
    idx_t candidate = 1;

    for (;;)
    {
        /* Compute 2*n-1 and 2*n+1. */
        seq_t p1 = (seq_t)candidate * 2 - 1; /* 2n-1 */
        seq_t p2 = p1 + 2;                   /* 2n+1 */

        /* A number is not composite if it is <=1 or it is prime. */
        bool p1_not_composite = (p1 <= (seq_t)1) || is_prime_big(p1);
        bool p2_not_composite = (p2 <= (seq_t)1) || is_prime_big(p2);

        /* Include candidate if at least one of p1 or p2 is composite. */
        if (!(p1_not_composite && p2_not_composite))
        {
            count++;
            if (count == n)
                return (seq_t)candidate;
        }

        candidate++;
    }
}
