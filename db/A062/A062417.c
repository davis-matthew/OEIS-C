/*
 * A062417  Integer part of log(n!)^((1 - 1/n)^(1/n)).
 * https://oeis.org/A062417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ⌊(log(n!))^((1-1/n)^(1/n))⌋ for n ≥ 1
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* with a(1) = 1 by definition   */

#include <math.h>

seq_t a(idx_t n)
{
    /* Special case: n = 1 is defined as 1 */
    if (n == 1) {
        return (seq_t)1;
    }

    /* Compute log(n!) using the lgamma function */
    long double log_fact = lgammal((long double)n + 1.0L);

    /* Compute the exponent (1 - 1/n)^(1/n)   */
    long double one_minus = 1.0L - 1.0L / (long double)n;
    long double exponent = powl(one_minus, 1.0L / (long double)n);

    /* Compute (log(n!))^exponent */
    long double value = powl(log_fact, exponent);

    /* Integer part – add a tiny epsilon to guard against rounding errors */
    const long double eps = 1e-12L;
    long long int_part = (long long)floorl(value + eps);

    /* Convert the integer part to seq_t and return */
    return (seq_t)int_part;
}
