/*
 * A062109  Expansion of g.f. ((1-x)/(1-2*x))^4.
 * https://oeis.org/A062109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(0) = 1 For n >= 1, a(n) = (n+5) * (n^2 + 13*n + 18) * 2^(n-5)
 *            / 3 (the division by 3 is exact)
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 23, u64 n <= 52, u128 n <= 113,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Convert the index to the sequence type first; this allows
       arbitrary‑precision arithmetic when seq_t is such a type. */
    seq_t ns = (seq_t)n;

    /* Compute (n+5)*(n^2 + 13*n + 18).  All intermediate values are
       held in seq_t to avoid overflow for arbitrary‑precision builds. */
    seq_t part1 = ns + 5;
    seq_t part2 = ns * ns + (seq_t)13 * ns + (seq_t)18;
    seq_t prod  = part1 * part2;

    /* The product is always divisible by 3. */
    prod /= 3;

    /* Multiply by 2^(n-5).  For n < 5 this is a division by 2^(5-n). */
    if (n >= 5)
        prod <<= (n - 5);
    else
        prod >>= (5 - n);

    return prod;
}
