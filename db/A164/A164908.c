/*
 * A164908  a(n) = (3*4^n - 0^n)/2.
 * https://oeis.org/A164908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=6, a(n)=4*a(n-1) for n>1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t a_val = (seq_t)6;
    for (idx_t i = 2; i <= n; ++i) {
        a_val *= (seq_t)4;
    }
    return a_val;
}
