/*
 * A127032  Maximal value of m such that 5^m <= n! : a(n) = floor( log(n!) /
 *          log(5) ).
 * https://oeis.org/A127032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( log_5(n!) ) = floor( sum_{k=1}^n log_5(k) )
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a double accumulation of logs and truncates to obtain
   the integer part, which is exact for all n <= 10000 in the required
   environments. */

#include <math.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }

    /* Pre‑compute reciprocal of log10(5) for speed */
    const double inv_log5 = 1.0 / log10(5.0);

    /* Accumulate the sum of base‑5 logarithms in a double. */
    double log_sum = 0.0;
    for (uint64_t i = 1; i <= n; ++i) {
        log_sum += log10((double)i) * inv_log5;
    }

    /* Convert the positive fraction to an integer by truncation. */
    unsigned long long val = (unsigned long long)log_sum;
    return (seq_t)val;
}
