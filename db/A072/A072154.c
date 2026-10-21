/*
 * A072154  Coordination sequence for the planar net 4.6.12.
 * https://oeis.org/A072154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; for n>0 let k=n/5, m=n%5; if m==0 then a(n)=12*k else
 *            a(n)=12*k+2*m+1
 * Range:     u8 n <= 106, u16 n <= 999, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 1;
    }
    idx_t k = n / 5;
    idx_t m = n % 5;
    if (m == 0) {
        return (seq_t)12 * k;
    } else {
        return (seq_t)12 * k + (seq_t)(2 * m + 1);
    }
}
