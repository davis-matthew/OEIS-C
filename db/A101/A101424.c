/*
 * A101424  Number of different cuboids with volume p^4 * q^n, where p,q are
 *          distinct prime numbers.
 * https://oeis.org/A101424
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (29 + 3*(-1)^n + 36*n + 10*n^2)/8
 * Range:     u8 n <= 3, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Direct computation of A101424: number of cuboids with volume p^4*q^n   */
seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                     /* convert index to seq_t      */
    seq_t base = 10*nn*nn + 36*nn + 29;      /* 10*n^2 + 36*n + 29          */
    seq_t numerator;
    if (n & 1)                               /* n odd -> subtract 3          */
        numerator = base - (seq_t)3;
    else                                     /* n even -> add 3              */
        numerator = base + (seq_t)3;
    return numerator / (seq_t)8;            /* integer division, exact      */
}
