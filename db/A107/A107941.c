/*
 * A107941  a(n) = (n+1)(n+2)^2*(n+3)^2*(n+4)^2*(n+5)(3n^2 + 13n + 15)/43200.
 * https://oeis.org/A107941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)(n+2)^2*(n+3)^2*(n+4)^2*(n+5)(3n^2 + 13n + 15)/43200
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 5, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;          /* convert index to the seq_t type */

    seq_t t1 = sn + 1;
    seq_t t2 = sn + 2;
    seq_t t3 = sn + 3;
    seq_t t4 = sn + 4;
    seq_t t5 = sn + 5;

    /* 3n^2 + 13n + 15  */
    seq_t t6 = 3 * sn * sn + 13 * sn + 15;

    /* Compute the numerator step by step */
    seq_t num = t1 * t5;          /* (n+1)(n+5)          */
    num *= t2 * t2;               /* * (n+2)^2           */
    num *= t3 * t3;               /* * (n+3)^2           */
    num *= t4 * t4;               /* * (n+4)^2           */
    num *= t6;                    /* * (3n^2 + 13n + 15) */

    /* Divide by the constant denominator */
    return num / (seq_t)43200;
}
