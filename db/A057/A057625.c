/*
 * A057625  a(n) = n! * sum 1/k! where the sum is over all positive integers k
 *          that divide n.
 * https://oeis.org/A057625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * sum_{d|n} 1/d! (n >= 1)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 403 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implementation uses the pre‑existing helper factorial(idx_t) */

#include <stdint.h>

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

seq_t a(idx_t n)
{
    /* the sequence is defined only for n >= 1; for n = 0 return 0   */
    if (n < 1) {
        return (seq_t)0;
    }

    /* factorial of n, used for all divisor terms */
    seq_t fact_n = factorial(n);

    seq_t sum = (seq_t)0;

    /* iterate over all divisors of n, two at a time up to sqrt(n)   */
    idx_t limit = 0;
    while ((limit + 1) * (limit + 1) <= n) {
        ++limit;
    }

    for (idx_t d = 1; d <= limit; ++d) {
        if (n % d == 0) {
            /* term for the divisor d          */
            seq_t fact_d = factorial(d);
            sum += fact_n / fact_d;

            /* term for the paired divisor n/d, if distinct */
            idx_t e = n / d;
            if (e != d) {
                seq_t fact_e = factorial(e);
                sum += fact_n / fact_e;
            }
        }
    }

    return sum;
}
