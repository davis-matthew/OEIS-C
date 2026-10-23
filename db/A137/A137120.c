/*
 * A137120  Numbers k such that k and k^2 use only the digits 3, 4, 5 and 6.
 * https://oeis.org/A137120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1)=6, a(n)=a(n-1)*10+6 for n>1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = 6;
    for (idx_t i = 1; i < n; ++i) {
        x = x * 10 + 6;
    }
    return x;
}
