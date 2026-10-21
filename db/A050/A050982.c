/*
 * A050982  5-idempotent numbers.
 * https://oeis.org/A050982
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n,5) * 5^(n-5) for n >= 5
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 5) {
        /* Below the offset no value is specified; return 0 */
        return (seq_t)0;
    }

    /* Compute binomial(n,5) = n*(n-1)*(n-2)*(n-3)*(n-4)/120 */
    seq_t bin = (seq_t)n;
    bin *= (seq_t)(n - 1);
    bin *= (seq_t)(n - 2);
    bin *= (seq_t)(n - 3);
    bin *= (seq_t)(n - 4);
    bin /= (seq_t)120;   /* 5! = 120 */

    /* Compute 5^(n-5) by repeated multiplication */
    seq_t pow5 = (seq_t)1;
    idx_t exp = n - 5;
    for (idx_t i = 0; i < exp; ++i) {
        pow5 *= (seq_t)5;
    }

    return bin * pow5;
}
