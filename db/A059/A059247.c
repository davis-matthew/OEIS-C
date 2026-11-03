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
 * Validated: standard, pass, 87 terms through n = 928 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where d(j) is the divisor-count function */
#include <stdint.h>
#include "../utils/gcd.h"

static seq_t sum_divisor_counts(idx_t n) {
    seq_t sum = 0;
    idx_t i = 1;
    while (i <= n) {
        idx_t q   = n / i;
        idx_t nxt = n / q + 1;          /* first i such that n/i < q */
        idx_t cnt = nxt - i;
        sum += (seq_t)q * (seq_t)cnt;
        i = nxt;
    }
    return sum;
}

seq_t a(idx_t n) {
    /* always valid for n >= 1 */
    seq_t sum = sum_divisor_counts(n);
    seq_t g   = gcd((seq_t)n, sum);
    return ((seq_t)n) / g;
}
