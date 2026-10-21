/*
 * A059247  Denominator of Sum_{j=1..n} d(j)/n, where d = number of divisors
 *          function (A000005).
 * https://oeis.org/A059247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = n / gcd( n , Σ_{j=1..n} d(j) )
 * Range:     u8 n <= 59, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd, sum_divisor_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where d(j) is the divisor-count function */
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/sum_divisor_counts.h"

seq_t a(idx_t n) {
    /* always valid for n >= 1 */
    seq_t sum = sum_divisor_counts(n);
    seq_t g   = gcd((seq_t)n, sum);
    return ((seq_t)n) / g;
}
