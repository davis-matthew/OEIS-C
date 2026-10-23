/*
 * A158540  a(n) = 22*n^2 - 1.
 * https://oeis.org/A158540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 22*n^2 - 1
 * Range:     u8 n <= 3, u16 n <= 54, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t res = sn * sn;
    res = res * (seq_t)22;
    res = res - (seq_t)1;
    return res;
}
