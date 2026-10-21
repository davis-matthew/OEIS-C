/*
 * A213857  Least m such that n! <= 3^m.
 * https://oeis.org/A213857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceiling( log_3(n!) ) = floor( Σ_{k=1..n} log_3(k) ) + 1 *
 *            * The function is implemented by summing the logarithms in base
 *            3 * using long double arithmetic. For the given n-range the
 *            approximation * error is far below 0.5 so the flooring operation
 *            is safe
 * Range:     u8 n <= 81, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* offset 1: a(1) = 1 */
    if (n < 1)
        return 0; /* undefined behaviour for n < 1 */

    const long double inv_log10_3 = 1.0L / log10l((long double)3);
    long double sum = 0.0L;

    /* sum log_3(k) for k=2..n */
    for (idx_t k = 2; k <= n; ++k)
        sum += (log10l((long double)k) * inv_log10_3);

    /* add a tiny epsilon to guard against round‐off that would push a
       value just below an integer down to the wrong integer after
       flooring.  The magnitude of log values is < 10, and the
       accumulation is at most ~80 000 for n=10 000, so 1e-12 is
       safely small. */
    const long double eps = 1.0e-12L;
    seq_t result = (seq_t)((long long)(sum + eps) + 1);
    return result;
}
