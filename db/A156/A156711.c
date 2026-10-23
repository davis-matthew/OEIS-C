/*
 * A156711  a(n) = 144*n^2 - 161*n + 45.
 * https://oeis.org/A156711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 144*n^2 - 161*n + 45
 * Range:     u8 n <= 1, u16 n <= 21, u32 n <= 5461, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return 144 * nn * nn - 161 * nn + 45;
}
