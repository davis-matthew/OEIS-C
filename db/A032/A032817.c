/*
 * A032817  Numbers whose set of base-5 digits is {1,4}.
 * https://oeis.org/A032817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 5*a(floor(n/2))+1 for n odd, * a(n) =
 *            5*a(floor((n-1)/2))+4 for n even, * with a(1)=1, a(2)=4. (a(0)
 *            is defined as 0 for internal use.)
 * Range:     u8 n <= 22, u16 n <= 190, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)0;
    if (n == 1u) return (seq_t)1;
    if (n == 2u) return (seq_t)4;

    if (n & 1u)       /* n odd */
        return (seq_t)5 * a(n >> 1) + (seq_t)1;
    else              /* n even */
        return (seq_t)5 * a((n - 1u) >> 1) + (seq_t)4;
}
