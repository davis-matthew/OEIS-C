/*
 * A047741  A discrete analog of Li(n): a(n) = round(Sum_{k=2..n} 1/log(k)).
 * https://oeis.org/A047741
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(Σ_{k=2..n} 1/log(k))
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return 0; /* undefined for n < 2, offset is 2 */

    double sum = 0.0;
    for (idx_t k = 2; k <= n; ++k)
        sum += 1.0 / log((double)k);

    /* round to nearest integer */
    long long r = (long long)(sum + 0.5);
    return (seq_t)r;
}
