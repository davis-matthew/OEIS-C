/*
 * A075358  a(n) = smallest (n+1)(n+2)...(n+k) that is >= n!.
 * https://oeis.org/A075358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A075358: a(n) = smallest (n+1)(n+2)…(n+k) that is ≥ n!   */
#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case: a(1) = 1 is handled by the general code below. */
    /* Compute n! as a seq_t. */
    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    /* Compute log(n!) = Σ log(i) for i = 2..n. */
    double target_log = 0.0;
    for (idx_t i = 2; i <= n; ++i)
        target_log += log((double)i);

    /* Find minimal k such that Σ_{j=1..k} log(n + j) ≥ log(n!). */
    idx_t k = 0;
    double sum_log = 0.0;
    while (sum_log < target_log)
    {
        ++k;
        sum_log += log((double)(n + k));
    }

    /* Compute the product (n+1)(n+2)…(n+k). */
    seq_t prod = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i)
        prod *= (seq_t)(n + i);

    return prod;
}
