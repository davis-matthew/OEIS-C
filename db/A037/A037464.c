/*
 * A037464  Bisection of A076605.
 * https://oeis.org/A037464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = max(largest_prime_factor(2*n-1),
 *            largest_prime_factor(2*n+1))
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gpf.h"

seq_t a(idx_t n)
{
    /* 2*n-1 and 2*n+1 are odd; n >= 1 (offset of the sequence) */
    idx_t low  = 2*n - 1;
    idx_t high = 2*n + 1;

    seq_t fp_low  = 0;
    seq_t fp_high = 0;

    /* gpf(1) is not defined by the utils: guard it explicitly */
    if (low > 1)  fp_low  = (seq_t)gpf((uint64_t)low);
    if (high > 1) fp_high = (seq_t)gpf((uint64_t)high);

    return (fp_low > fp_high) ? fp_low : fp_high;
}
