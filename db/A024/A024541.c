/*
 * A024541  [ sum of 1/{k*sqrt(2)} ], k = 1,2,...,n, where {x} := x - [ x ].
 * https://oeis.org/A024541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( Σ_{k=1..n} 1/{k*sqrt(2)} ), where {x}=x-[x]
 * Range:     u8 n <= 46, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                /* off‑by‑one safety */

    const long double sqrt2 = sqrtl(2.0L);      /* irrational constant */
    long double frac = 0.0L;                    /* fractional part of k*sqrt2 */
    long double sum  = 0.0L;                    /* accumulated sum of reciprocals */

    for (idx_t k = 1; k <= n; ++k)
    {
        frac += sqrt2;                          /* add sqrt2 */
        frac -= floorl(frac);                   /* keep only fractional part */
        sum  += 1.0L / frac;                    /* add 1/frac to total sum */
    }

    /* Convert final floating accumulated sum to integer seq_t safely. */
    return (seq_t)((long long)floorl(sum));
}
