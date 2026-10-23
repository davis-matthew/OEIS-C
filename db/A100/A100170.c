/*
 * A100170  Structured pentagonal hexacontahedral numbers (vertex structure
 *          10).
 * https://oeis.org/A100170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (171*n^3 - 243*n^2 + 78*n)/6
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t result = (seq_t)171 * nn * nn * nn;
    result -= (seq_t)243 * nn * nn;
    result += (seq_t)78 * nn;
    result /= (seq_t)6;
    return result;
}
