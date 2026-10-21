/*
 * A052881  Expansion of e.g.f. log(1/(1-x))*x/(1-x).
 * https://oeis.org/A052881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n! * Sum_{i=1}^{n-1} 1/i Recurrence: a(0)=0, a(1)=0, a(n)
 *            = Sum_{i=1}^{n-1} n!/i (n≥2) The sequence is the sequence of n!
 *            times the harmonic number H_{n-1}
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* trivial cases */
    if (n < 2)
        return (seq_t)0;

    /* compute n! */
    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    /* sum n!/i over i = 1..n-1 */
    seq_t sum = (seq_t)0;
    for (idx_t i = 1; i <= n - 1; ++i)
        sum += fact / (seq_t)i;

    return sum;
}
