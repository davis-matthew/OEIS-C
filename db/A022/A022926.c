/*
 * A022926  Number of powers of 7 between 2^n and 2^(n+1).
 * https://oeis.org/A022926
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(log_7 2^(n + 1)) - floor(log_7 2^n)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Constant value log_7(2) = ln(2)/ln(7) */
    const long double LOG7_2 = logl(2.0L) / logl(7.0L);
    /* Small epsilon to guard against rounding errors */
    const long double EPS = 1e-18L;

    const long double n_ld = (long double)n;
    /* floor(n * log_7 2) */
    uint64_t f0 = (uint64_t)floorl(n_ld * LOG7_2 + EPS);
    /* floor((n + 1) * log_7 2) */
    uint64_t f1 = (uint64_t)floorl((n_ld + 1.0L) * LOG7_2 + EPS);

    /* a(n) is the difference, which is always 0 or 1 */
    uint64_t diff = f1 - f0;
    return (seq_t)diff;
}
