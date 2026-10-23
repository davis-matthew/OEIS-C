/*
 * A131712  Period 4: repeat [1, 3, 7, 9].
 * https://oeis.org/A131712
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = {1, 3, 7, 9}[n mod 4] (a(n) = a(n-4) for n > 3, with
 *            a(0)=1, a(1)=3, a(2)=7, a(3)=9)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t tbl[4] = { (seq_t)1, (seq_t)3, (seq_t)7, (seq_t)9 };
    return tbl[n & 3];
}
