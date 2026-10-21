/*
 * A211510  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          w^2 = x*y - 2n.
 * https://oeis.org/A211510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{(w,x,y) : 1≤w,x,y≤n, w² = x*y - 2n}|
 * Range:     u8 n <= 107, u16 n <= 1024, u32 n <= 1024, u64 n <= 1024,
 *            u128 n <= 1024, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Count ordered triples (w,x,y) with 1<=w,x,y<=n satisfying w^2 = x*y - 2n.  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* a(0)=0 */
    unsigned long long count = 0;
    for (idx_t w = 1; w <= n; ++w)
    {
        unsigned long long w2 = (unsigned long long)w * (unsigned long long)w;
        unsigned long long m   = w2 + (unsigned long long)2 * n;   /* x*y = m */

        /* Iterate through divisors d of m up to min(n, sqrt(m)) */
        for (unsigned long long d = 1; d <= n && d * d <= m; ++d)
        {
            if (m % d != 0) continue;
            unsigned long long e = m / d;
            if (e > n) continue;          /* co‑factor exceeds bound */
            if (d == e)      count += 1;  /* (d,d) counted once */
            else             count += 2;  /* ordered pair (d,e) and (e,d) */
        }
    }
    return (seq_t)count;
}
