/*
 * A010799  a(n) = n!*(n+1)!*(n+2)!*(n+3)!*(n+4)! / ( 2!*3!*4!*5! ).
 * https://oeis.org/A010799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n!*(n+1)!*(n+2)!*(n+3)!*(n+4)! / (2!*3!*4!*5!)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    seq_t res = 1;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t p = (seq_t)i;
        p *= (seq_t)(i + 1);
        p *= (seq_t)(i + 2);
        p *= (seq_t)(i + 3);
        p *= (seq_t)(i + 4);
        res *= p;
    }
    return res;
}
