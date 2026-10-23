/*
 * A104150  Shifted factorial numbers: a(0)=0, a(n) = (n-1)!.
 * https://oeis.org/A104150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 0, a(n) = (n-1)! (for n ≥ 1)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t res = 1;
    for (idx_t i = 2; i < n; ++i) {
        res = res * (seq_t)i;
    }
    return res;
}
