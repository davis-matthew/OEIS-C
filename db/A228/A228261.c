/*
 * A228261  Number of third differences of arrays of length 5 of numbers in
 *          0..n.
 * https://oeis.org/A228261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 35*n^2 + n + 1 for n>1; a(1)=31
 * Range:     u8 n <= 2, u16 n <= 43, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)31;
    /* For n>=2 use the empirical quadratic formula */
    seq_t x = (seq_t)n;
    return 35 * x * x + x + 1;
}
