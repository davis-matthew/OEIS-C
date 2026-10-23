/*
 * A128939  Maximal product over partitions of n into parts of the form 3k+1.
 * https://oeis.org/A128939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A128939
   Formula: a(n) = 1 if n<4,
             4^⌊n/4⌋            if n ≠ 4m+3,
             7*4^(⌊n/4⌋−1)      if n = 4m+3
*/

seq_t a(idx_t n)
{
    /* Return 1 for n < 4 (invalid or trivial cases). */
    if (n < 4) return (seq_t)1;

    /* Compute q = floor(n/4). */
    idx_t q = n / 4;

    /* Determine exponent of 4. */
    idx_t exp = (n % 4 == 3) ? (q - 1) : q;

    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        result *= (seq_t)4;

    if (n % 4 == 3)
        result *= (seq_t)7;

    return result;
}
