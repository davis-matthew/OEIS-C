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
 * Helpers:   floor_sqrt, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"
#include "../utils/gcd.h"

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
