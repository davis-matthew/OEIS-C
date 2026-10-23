/*
 * A132270  a(n) = floor((n^7-1)/(7*n^6)), which is the same as integers
 *          repeated 7 times.
 * https://oeis.org/A132270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n-1)/7) (equivalent to the given rational form)
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is guaranteed >= 1 due to the sequence offset */
    seq_t nn = (seq_t)n;
    return (nn - 1) / 7;
}
