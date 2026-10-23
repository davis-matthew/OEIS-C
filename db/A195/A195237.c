/*
 * A195237  Number of lower triangles of a 7 X 7 0..n array with each element
 *          differing from all of its diagonal, vertical, antidiagonal and
 *          horizontal neighbors by one or less.
 * https://oeis.org/A195237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2021888119*n - 2469384741 for n>4 Listed values:
 *            a(1)=268435456, a(2)=1663418313, a(3)=3601738548,
 *            a(4)=5618308863
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* not part of the sequence */
    if (n == 1) return (seq_t)268435456ULL;
    if (n == 2) return (seq_t)1663418313ULL;
    if (n == 3) return (seq_t)3601738548ULL;
    if (n == 4) return (seq_t)5618308863ULL;

    /* n >= 5 */
    seq_t m = (seq_t)2021888119ULL;
    return m * (seq_t)n - (seq_t)2469384741ULL;
}
