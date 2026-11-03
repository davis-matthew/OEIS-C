/*
 * A157228  Number of primitive inequivalent inclined square sublattices of
 *          square lattice of index n.
 * https://oeis.org/A157228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered coprime positive pairs (x,y) with 0 <
 *            x < y and x^2 + y^2 = n. (i.e. partitions of n into two distinct
 *            coprime squares)
 * Range:     u8 n <= 63, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 103 terms through n = 4943 (2026-11-01)
 * Helpers:   gcd, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* Convert the index to the arithmetic type */
    seq_t NN = (seq_t)n;
    /* Auxiliary variables */
    seq_t count = 0;

    /* Compute floor(sqrt(NN)) only once */
    seq_t sqrt_n = floor_sqrt(NN);

    /* Enumerate all candidate larger square roots y */
    seq_t y = 1;
    while (y <= sqrt_n) {
        seq_t y2 = y * y;            /* y^2 */
        if (y2 > NN)                 /* safety, should never happen */
            break;

        seq_t rem = NN - y2;         /* n - y^2 = x^2 */
        seq_t x   = floor_sqrt(rem); /* floor(sqrt(rem)) */

        if (x * x == rem) {          /* rem is a perfect square */
            if (x != 0 && x < y) {   /* distinct positive squares */
                /* Check coprimeness */
                if (gcd(x, y) == (seq_t)1)
                    count += 1;
            }
        }
        ++y;
    }
    return count;
}
