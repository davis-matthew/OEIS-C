/*
 * A020922  Expansion of 1/(1-4*x)^(11/2).
 * https://oeis.org/A020922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binom(n+5,5) * binom(2*n+10, n+5) / binom(10,5)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 10, u64 n <= 24, u128 n <= 53,
 *            big any n
 * Helpers:   binom, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A020922 – Expansion of 1/(1-4*x)^(11/2) */

#include "../utils/binom.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* binom(10,5) = 252 */
    const seq_t DENOM = 252;
    seq_t denom = DENOM;

    seq_t part1 = binom(n + 5, 5);
    seq_t part2 = binom(2 * n + 10, n + 5);

    /* cancel common factors with the denominator to prevent overflow */
    seq_t g = gcd(part1, denom);
    part1 /= g;
    denom  /= g;

    g = gcd(part2, denom);
    part2 /= g;
    denom  /= g;   /* denom should now be 1 */

    return part1 * part2;
}
