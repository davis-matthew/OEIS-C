/*
 * A107941  a(n) = (n+1)(n+2)^2*(n+3)^2*(n+4)^2*(n+5)(3n^2 + 13n + 15)/43200.
 * https://oeis.org/A107941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)(n+2)^2*(n+3)^2*(n+4)^2*(n+5)(3n^2 + 13n + 15)/43200
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 21, u64 n <= 217, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 980 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* the code below runs in wide_t: 128 bits at the fixed widths (never narrower
   than seq_t), seq_t itself for big, so values that pass 2^64 on the way to a
   term that fits stay exact; a() returns the term in seq_t */
#ifdef __cplusplus
typedef seq_t wide_t;
#else
typedef __typeof__((seq_t)0 + (__int128)0) wide_t;
#endif
#define seq_t wide_t

seq_t a_wide(idx_t n)
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

#undef seq_t
seq_t a(idx_t n)
{
    return (seq_t)a_wide(n);
}
