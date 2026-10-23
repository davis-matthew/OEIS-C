/*
 * A022923  Number of integers m such that 7^n < 2^m < 7^(n+1).
 * https://oeis.org/A022923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+1)*log2(7)) - floor(n*log2(7))
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This computes the number of integers m with 7^n < 2^m < 7^(n+1). */
#include <math.h>

seq_t a(idx_t n)
{
    /* Use long double for higher precision of log2(7). */
    static const long double LOG2_7 = log2l(7.0L);

    /* Compute floor of n*α and (n+1)*α.  The values are small (≤ 28070 for n≤10000). */
    unsigned long long f_n    = (unsigned long long)( (long double)n       * LOG2_7 );
    unsigned long long f_np1  = (unsigned long long)( ((long double)n)+1.0L ) * LOG2_7;

    /* Difference gives the desired count (always 2 or 3). */
    return (seq_t)(f_np1 - f_n);
}
