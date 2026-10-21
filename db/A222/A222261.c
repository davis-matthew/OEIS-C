/*
 * A222261  Lexicographically earliest injective sequence of positive integers
 *          such that the sum of 10 consecutive terms is always divisible by
 *          10.
 * https://oeis.org/A222261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n if n%5 != 0 else 2*n - 5
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n % 5) == 0)
        return ((seq_t)n * 2) - 5;
    else
        return (seq_t)n;
}
