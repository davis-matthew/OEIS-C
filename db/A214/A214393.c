/*
 * A214393  Numbers of the form (4k+3)^2+4 or (4k+5)^2-8.
 * https://oeis.org/A214393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n*(n+3)+6*(-1)^n+7
 * Range:     u8 n <= 6, u16 n <= 126, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1)-2*a(n-3)+a(n-4), with a(0)=13, a(1)=17, a(2)=53, a(3)=73 */

seq_t a(idx_t n)
{
    /* compute 4*n*(n+3) */
    seq_t s = (seq_t)n * (seq_t)(n + 3);   /* n*(n+3) */
    s *= 4;                                /* 4*n*(n+3) */
    s += 7;                                /* +7 */

    /* add 6*(-1)^n: +6 when n is even, -6 when n is odd */
    if ((n & 1) == 0)
        s += 6;
    else
        s -= 6;   /* base - 6 is safe because base >= 23 for odd n ≥ 1 */

    return s;
}
