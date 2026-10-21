/*
 * A060013  New record highs reached in A060000.
 * https://oeis.org/A060013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - 3 for n > 5 Initial terms: a(1)=1, a(2)=2,
 *            a(3)=3, a(4)=5, a(5)=9
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)3;
    if (n == 4) return (seq_t)5;
    if (n == 5) return (seq_t)9;

    seq_t val = (seq_t)9;           /* a(5) */
    for (idx_t i = 6; i <= n; ++i)  /* iterate up to n */
        val = val * (seq_t)2 - (seq_t)3;

    return val;
}
