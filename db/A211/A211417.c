/*
 * A211417  a(n) = (30*n)!*n!/((15*n)!*(10*n)!*(6*n)!).
 * https://oeis.org/A211417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (30*n)!*n! / ((15*n)!*(10*n)!*(6*n)!)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 0, u64 n <= 1, u128 n <= 3,
 *            big any n
 * Helpers:   binomial, seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t nn = n;

    seq_t t1 = binomial(30 * nn, 15 * nn);   /* (30n choose 15n)   */
    seq_t t2 = binomial(15 * nn, 5 * nn);    /* (15n choose 5n)    */
    seq_t den = binomial(6 * nn, nn);        /* (6n choose n)      */

    /* Cancel common factors between numerator and denominator
       to avoid overflow on fixed‑width builds. */
    seq_t g = seq_gcd(t2, den);
    t2 /= g;
    den /= g;

    g = seq_gcd(t1, den);
    t1 /= g;
    den /= g;

    /* After gcd reductions den should be 1. */
    return t1 * t2;
}
