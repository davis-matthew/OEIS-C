/*
 * A184904  n+floor(ns/r)+floor(nt/r), where r=2^(1/2), s=2^(1/3), t=2^(1/5).
 * https://oeis.org/A184904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*2^(-1/6)) + floor(n*2^(-3/10))
 * Range:     u8 n <= 94, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

/* seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n)
{
    /* Pre‑calculated constants for the irrational coefficients 2^(-1/6) and 2^(-3/10).
       The values are evaluated in long double precision so that the product with
       n is accurate enough for n up to at least 10^4. */
    static const long double alpha1 = 1.0L / powl(2.0L, 1.0L / 6.0L); /* 2^(-1/6) */
    static const long double alpha2 = 1.0L / powl(2.0L, 3.0L / 10.0L);/* 2^(-3/10) */

    long double nd = (long double)n;

    /* Compute the two floor terms. Their results are guaranteed to be < n
       (since the coefficients are < 1), so they comfortably fit in 64‑bit
       unsigned integers. */
    unsigned long long f1 = (unsigned long long)floorl(nd * alpha1);
    unsigned long long f2 = (unsigned long long)floorl(nd * alpha2);

    /* Return the sum, promoting each component to seq_t. */
    return (seq_t)n + (seq_t)f1 + (seq_t)f2;
}
