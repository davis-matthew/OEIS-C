/*
 * A159517  Numerator of Hermite(n, 8/15).
 * https://oeis.org/A159517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 15^n * Hermite(n,8/15) Recurrence: a(0)=1, a(1)=16,
 *            a(n+1)=16*a(n)-450*n*a(n-1) for n>=1
 * Range:     i8 n <= 1, i16 n <= 4, i32 n <= 6, i64 n <= 11, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef _A159517_H_
#define _A159517_H_

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)16;

    seq_t a0 = (seq_t)1;      /* a(0) */
    seq_t a1 = (seq_t)16;     /* a(1) */

    for (idx_t i = 1; i < n; ++i)   /* compute a(i+1) */
    {
        /* a(i+1) = 16*a(i) - 450*i*a(i-1) */
        seq_t next = (seq_t)16 * a1 - (seq_t)450 * (seq_t)i * a0;
        a0 = a1;
        a1 = next;
    }

    return a1;
}

#endif /* _A159517_H_ */
