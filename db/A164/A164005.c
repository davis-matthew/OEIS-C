/*
 * A164005  Zero together with row 5 of the array in A163280.
 * https://oeis.org/A164005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 (n = 0) 7 (n = 1) 14 (n = 2) n*(n + 4) (n >= 3)
 * Range:     u8 n <= 14, u16 n <= 254, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 7;
    if (n == 2) return 14;

    seq_t nn = (seq_t)n;
    return nn * (nn + 4);
}
