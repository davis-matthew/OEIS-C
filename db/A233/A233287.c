/*
 * A233287  a(n) = lcm_{i=1..n} A001177(i); partial LCM-products of Fibonacci
 *          entry points.
 * https://oeis.org/A233287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 36, u32 n <= 66, u64 n <= 162,
 *            u128 n <= 352, big any n
 * Helpers:   entry_point, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A233287
 * Formula: a(1) = 1; a(n) = lcm( A001177(n), a(n-1) ) for n > 1
 * where A001177(n) is the smallest k such that n divides Fib(k). */

#include <stddef.h>          /* size_t */
#include "../utils/gcd.h"   /* gcd(seq_t a, seq_t b) */
#include "../utils/entry_point.h"

/* External function required by the build. */
seq_t
a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;         /* undefined for 0, but safe */

    seq_t lcm_val = (seq_t)1;    /* a(1) = 1 */

    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t ep = entry_point(i);          /* A001177(i) */
        seq_t g  = gcd(lcm_val, ep);        /* gcd with current LCM */
        lcm_val = (lcm_val / g) * ep;       /* lcm(lcm_val, ep) */
    }

    return lcm_val;
}
