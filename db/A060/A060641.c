/*
 * A060641  Surround numbers of a length 2n zig-zag.
 * https://oeis.org/A060641
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*n^2 + 46*n + 29 (valid for n > 1)
 * Range:     u8 n <= 2, u16 n <= 78, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;            // behaviour below the offset is irrelevant
    seq_t nn = (seq_t)n;
    return (seq_t)10 * (nn * nn) + (seq_t)46 * nn + (seq_t)29;
}
