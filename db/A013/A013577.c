/*
 * A013577  Minimal scope of an (n,5) difference triangle.
 * https://oeis.org/A013577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard
 * Formula:   a(n) = 15*n + 4 for n >= 2; a(1) = 17
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)17;
    seq_t sn = (seq_t)n;
    return sn * 15 + 4;
}
