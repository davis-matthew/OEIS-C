/*
 * A022928  Number of 5^m between 3^n and 3^(n+1).
 * https://oeis.org/A022928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n+1)*log(3)/log(5)) - floor(n*log(3)/log(5))
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the number of powers 5^m lying strictly between 3^n and 3^(n+1) */

#include <math.h>

seq_t a(idx_t n)
{
    /* 80‑bit long double gives ~19 decimal digits of precision,
       enough for n up to about 10⁴ when only 0/1 output is required. */
    static const long double alpha = logl(3.0L) / logl(5.0L);

    /* Compute the two floor values in long double precision. */
    long double f1 = (n + 1) * alpha;
    long double f0 = (long double)n * alpha;

    /* Subtract and cast the small integer result to seq_t. */
    seq_t cnt = (seq_t)((long long)(floorl(f1) - floorl(f0)));
    return cnt;
}
