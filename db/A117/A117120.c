/*
 * A117120  a(1)=1. a(n) is smallest positive integer not occurring earlier in
 *          the sequence where a(n) is congruent to -1 (mod a(n-1)).
 * https://oeis.org/A117120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=1, a(2)=2, a(3)=3; for n>=4: if n even a(n)=2*a(n/2)+1,
 *            else a(n)=2*a(n/2)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)3;

    if ((n & 1) == 0)          /* even */
        return ((seq_t)2) * a(n / 2) + (seq_t)1;
    else                       /* odd, n>3 */
        return ((seq_t)2) * a(n / 2);
}
