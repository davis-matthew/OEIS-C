/*
 * A010201  Continued fraction for sqrt(142).
 * https://oeis.org/A010201
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0)=11, a(1)=1; multiplicative with a(2)=10, a(2^e)=22 for
 *            e>=2, a(p^e)=1 for odd prime p
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)11;
    if (n == 1) return (seq_t)1;
    idx_t m = n;
    int e = 0;
    while ((m & (idx_t)1) == 0) {   /* count factors of 2 */
        e++;
        m >>= 1;
    }
    if (e == 0) return (seq_t)1;    /* odd n > 1 */
    if (e == 1) return (seq_t)10;   /* 2 * odd */
    return (seq_t)22;               /* 4*odd or higher powers of 2 */
}
