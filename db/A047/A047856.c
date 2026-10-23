/*
 * A047856  a(n) = A047848(8, n).
 * https://oeis.org/A047856
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11^n + 9)/10; recurrence: a(0)=1, a(n)=11*a(n-1)-9 for
 *            n>0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= n; ++i) {
        res = (seq_t)11 * res - (seq_t)9;
    }
    return res;
}
