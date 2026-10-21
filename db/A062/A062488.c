/*
 * A062488  Integer part of (Product(n^((1 + log(i))/(1 + i^2)), {i, 1, n})).
 * https://oeis.org/A062488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊∏_{i=1}^{n} n^((1+log(i))/(1+i²))⌋
 * Range:     u8 n <= 24, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The product simplifies to n^{S(n)} where S(n)=∑_{i=1}^{n}(1+log(i))/(1+i²). */

#include <math.h>

seq_t a(idx_t n)
{
    if (n == 0) return 1;            /* a(0) = 1 */

    long double sum = 0.0L;
    for (idx_t i = 1; i <= n; ++i) {
        long double x = (long double)i;
        sum += (1.0L + logl(x)) / (1.0L + x * x);
    }

    /* Compute n^{S(n)} as exp(S(n) * log(n)) */
    long double val   = expl(sum * logl((long double)n));

    /* Small epsilon to mitigate rounding towards the floor */
    const long double eps = 1e-12L;

    long long  lld = (long long)floorl(val + eps);
    return (seq_t)lld;
}
