/*
 * A002275  Repunits: (10^n - 1)/9. Often denoted by R_n.
 * https://oeis.org/A002275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice, core
 * Formula:   a(n) = 10*a(n-1) + 1, a(0) = 0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)0;
    for (idx_t i = 0; i < n; i++) {
        result = result * (seq_t)10 + (seq_t)1;
    }
    return result;
}
