/*
 * A090381  Expansion of (1+4x+7x^2)/((1-x)^2*(1-x^2)).
 * https://oeis.org/A090381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n*(n+1)+(1+(-1)^n)/2
 * Range:     u8 n <= 8, u16 n <= 147, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;            /* convert to seq_t for arithmetic   */
    seq_t res = 3 * x * (x + 1);   /* 3*n*(n+1)                         */
    if ((x & 1) == 0)              /* add 1 when n is even              */
        res += 1;
    return res;
}
