/*
 * A077750  Least significant digit of A077749(n).
 * https://oeis.org/A077750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(0)=0; a(4n-3)=0, a(4n-2)=2, a(4n-1)=6, a(4n)=4 for n>0 (least
 *            significant digit of A077749(n))
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t table[4] = { 0, 2, 6, 4 };
    return table[n & 3];
}
