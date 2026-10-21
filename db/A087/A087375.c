/*
 * A087375  Smallest n-th power > n!.
 * https://oeis.org/A087375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest n‑th power that exceeds n! Equivalently
 *            a(n) = A214046(n)^n for n > 1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   pow_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/pow_n.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;   /* a(1) = 2 */

    /* Compute n! as seq_t */
    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i) fact *= (seq_t)i;

    /* Sum of logarithms in base 10 (long double for accuracy) */
    long double log10fact = 0.0L;
    for (idx_t i = 2; i <= n; ++i)
        log10fact += log10((long double)i);

    /* Rough nth‑root estimate of n! */
    long double root_est_ld = powl(10.0L, log10fact / (long double)n);
    idx_t r_est = (idx_t)floor(root_est_ld + 1e-12L);
    if (r_est < 1) r_est = 1;

    /* Refine estimate by checking powers */
    seq_t pow_val = pow_n((seq_t)r_est, n);
    while (pow_val <= fact) {
        ++r_est;
        pow_val = pow_n((seq_t)r_est, n);
    }

    return pow_val;   /* r_est^n is the desired term a(n) */
}
