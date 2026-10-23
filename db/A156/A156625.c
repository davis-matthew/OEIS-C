/*
 * A156625  Floor(integral of log(x) from 1 to n).
 * https://oeis.org/A156625
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(n*log(n)) - n + 1 (n ≥ 2)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* The sequence is undefined for n < 2 – return 0 for safety */
    if (n < 2)
        return 0;

    /* Compute floor(n*log(n)) using long double arithmetic. */
    /* floorl ensures the mathematical floor and avoids truncation
       towards zero for negative values. */
    long double val = floorl((long double)n * logl((long double)n));

    /* Convert the result to a native integer type that fits the
       expected range (for n ≤ 10000 the result is < 1e5). */
    uint64_t f = (uint64_t)val;

    /* Final formula: a(n) = floor(n*log(n)) - n + 1 */
    uint64_t result = f - n + 1;

    /* Cast to seq_t – the constructor from a native integer is
       guaranteed to be available on all supported build targets. */
    return (seq_t)result;
}
