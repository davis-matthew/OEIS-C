/*
 * A116756  Number of permutations of length n which avoid the patterns 1234,
 *          3421, 4312.
 * https://oeis.org/A116756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (49*n^2 - 325*n + 540)/2 for n >= 5 a(1) = 1 a(2) = 2
 *            a(3) = 6 a(4) = 21
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)6;
    if (n == 4) return (seq_t)21;

    seq_t nn = (seq_t)n;
    seq_t val = (seq_t)49 * nn * nn - (seq_t)325 * nn + (seq_t)540;
    val /= (seq_t)2;
    return val;
}
