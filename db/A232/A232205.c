/*
 * A232205  a(0)=1; thereafter a(n) = n*a(n-1) if n is even, otherwise a(n) =
 *          2*n*a(n-1).
 * https://oeis.org/A232205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!*2^⌊(n+1)/2⌋
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= n; ++i) {
        if ((i & 1ULL) == 0) {
            res *= (seq_t)i;
        } else {
            res *= ((seq_t)i * (seq_t)2);
        }
    }
    return res;
}
