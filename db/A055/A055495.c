/*
 * A055495  Numbers k such that there exists a pair of mutually orthogonal
 *          Latin squares of order k.
 * https://oeis.org/A055495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n+2 for 1<=n<=3, a(n) = n+3 for n>=4 (i.e. all integers
 *            k>=3 except 6)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < (idx_t)4)
        return (seq_t)(n + (idx_t)2);
    else
        return (seq_t)(n + (idx_t)3);
}
