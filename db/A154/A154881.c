/*
 * A154881  First prime in a consecutive sequence of 6 primes such that, when
 *          taken as ordered x,y,z triples, the Cartesian distance between the
 *          two points is an integer.
 * https://oeis.org/A154881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 84, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A154881: first prime of a block of 6 consecutive primes for which the
   Cartesian distance between (p1,p2,p3) and (p4,p5,p6) is an integer.
   The algorithm generates primes on the fly, tests each consecutive
   block of six primes, and returns the prime p1 when the distance
   sqrt((p4-p1)^2 + (p5-p2)^2 + (p6-p3)^2) is integral. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/next_prime.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;        /* offset is 1 */

    idx_t hit = 0;

    /* first six consecutive primes */
    uint64_t p1 = next_prime(1);  /* 2 */
    uint64_t p2 = next_prime(p1);
    uint64_t p3 = next_prime(p2);
    uint64_t p4 = next_prime(p3);
    uint64_t p5 = next_prime(p4);
    uint64_t p6 = next_prime(p5);

    while (true)
    {
        /* distance squared between the two triples */
        seq_t d1 = (seq_t)p4 - (seq_t)p1;
        seq_t d2 = (seq_t)p5 - (seq_t)p2;
        seq_t d3 = (seq_t)p6 - (seq_t)p3;
        seq_t dist_sq = d1 * d1 + d2 * d2 + d3 * d3;

        seq_t d   = floor_sqrt(dist_sq);
        if (d * d == dist_sq)          /* integer distance found */
        {
            hit++;
            if (hit == n) return (seq_t)p1;
        }

        /* slide window by one prime */
        p1 = p2;
        p2 = p3;
        p3 = p4;
        p4 = p5;
        p5 = p6;
        p6 = next_prime(p6);
    }
}
