/*
 * A127037  Maximal value of m such that 19^m <= n! : a(n) = floor( log(n!) /
 *          log(19) ).
 * https://oeis.org/A127037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(log_19(n!)) = floor( ln(n!) / ln(19) )
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A127037 */

#include <math.h>

seq_t a(idx_t n)
{
    /* compute log base 19 of n! by summing ln(i)/ln(19) for i = 2..n */
    if (n <= 1) return 0;

    const long double log19 = logl((long double)19.0L);
    long double sum = 0.0L;
    for (idx_t i = 2; i <= n; ++i)
        sum += logl((long double)i) / log19;

    /* Add a tiny epsilon to avoid rounding down when the result is
       extremely close to an integer due to floating‑point errors. */
    const long double eps = 1e-12L;
    idx_t m = (idx_t)floorl(sum + eps);

    return (seq_t)m;
}
