/*
 * A194501  Number of lower triangles of an n X n 0..4 array with each element
 *          unequal to the sum mod 5 of its horizontal and vertical neighbors.
 * https://oeis.org/A194501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4^((n*(n+1))/2) for n≥5; special cases for n=1..4
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle small n explicitly to match the sequence. */
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)64;
    if (n == 3) return (seq_t)4160;
    if (n == 4) return (seq_t)1048560;

    /* For n ≥ 5 use the general formula. */
    seq_t exp = (seq_t)n * ((seq_t)n + 1) / 2;
    return big_pow((seq_t)4, exp);
}
