/*
 * A184536  a(n) = floor(1/{(1+n^4)^(1/4)}), where {} = fractional part.
 * https://oeis.org/A184536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(1/{(1+n^4)^(1/4)}). For n ≥ 2 this equals 4*n^3,
 *            whereas a(1) = 5
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case for n = 1 */
    if (n == 1)
        return (seq_t)5;

    /* n is guaranteed to be ≥ 1 as per OEIS offset */
    seq_t nn = (seq_t)n;

    /* Compute 4 * n^3 */
    seq_t result = 4 * nn * nn * nn;
    return result;
}
