/*
 * A153284  a(n) = n + Sum_{j=1..n-1} (-1)^j * a(j) for n >= 2, a(1) = 1.
 * https://oeis.org/A153284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n + Σ_{j=1}^{n-1} (-1)^j * a(j), with a(1)=1
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Simplified form: a(n)=1 for n=1 or even; a(n)=3 for odd n>1 */
seq_t a(idx_t n)
{
    if ((n & (idx_t)1) == 0)      /* even n */
        return (seq_t)1;
    if (n == 1)                   /* n == 1 */
        return (seq_t)1;
    return (seq_t)3;               /* odd n > 1 */
}
