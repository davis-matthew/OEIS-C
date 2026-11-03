/*
 * A152770  Sum of proper divisors minus the number of proper divisors of n:
 *          a(n) = sigma(n) - n - d(n) + 1.
 * https://oeis.org/A152770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma(n) - n - d(n) + 1 where sigma(n) is the sum of all
 *            positive divisors of n and d(n) is the number of positive
 *            divisors of n. a(n) is the sum of proper divisors of n minus the
 *            number of proper divisors of n plus 1
 * Range:     u8 n <= 167, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 96 terms through n = 972 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute sigma(n) and d(n) in a single pass.  Complexity O(sqrt(n)). */
static inline seq_t sigma_minus_n_and_divisors(idx_t n, seq_t *d_out)
{
    seq_t sum = 0;          /* sigma(n) */
    idx_t  d   = 0;         /* number of divisors */
    idx_t   i;
    for (i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            /* i is a divisor */
            ++d;
            sum += (seq_t)i;
            /* n/i is another divisor unless i==n/i */
            if (i != n / i) {
                ++d;
                sum += (seq_t)(n / i);
            }
        }
    }
    *d_out = (seq_t)d;
    /* return sigma(n) */
    return sum;
}

seq_t a(idx_t n)
{
    if (n < 1) {
        /* Undefined for n < 1; return 0 for safety. */
        return 0;
    }
    seq_t d;
    seq_t sigma_n = sigma_minus_n_and_divisors(n, &d);
    /* proper sum = sigma(n) - n */
    seq_t proper_sum = sigma_n - (seq_t)n;
    /* a(n) = proper_sum - d + 1 */
    seq_t result = proper_sum + 1 - d;
    return result;
}
