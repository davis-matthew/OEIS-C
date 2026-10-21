/*
 * A065164  Permutation t->t+1 of Z, folded to N.
 * https://oeis.org/A065164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=2; for n>1, a(n) = n+2 if n is even, else a(n)=n-2
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)2;

    if ((n & (idx_t)1) == 0)           /* n even */
        return (seq_t)(n + 2);
    else                               /* n odd */
        return (seq_t)(n - 2);
}
