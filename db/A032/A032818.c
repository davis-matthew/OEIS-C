/*
 * A032818  Numbers whose set of base-6 digits is {1,4}.
 * https://oeis.org/A032818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 6*a(floor(n/2))+1 for n odd, a(n) = 6*a(floor((n-1)/2))+4
 *            for n even, with a(1)=1, a(2)=4
 * Range:     u8 n <= 14, u16 n <= 158, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n & (idx_t)1) /* odd */
        return (seq_t)6 * a(n / (idx_t)2) + (seq_t)1;
    else /* even */
        return (seq_t)6 * a((n - (idx_t)1) / (idx_t)2) + (seq_t)4;
}
