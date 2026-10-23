/*
 * A131133  a(0)=1; for n > 0, a(n) = (1/n + 1/a(n-1))*lcm(n, a(n-1)).
 * https://oeis.org/A131133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>0, a(n) = (a(n-1)+n)/gcd(a(n-1),n)
 * Range:     u8 n <= 79, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t prev = (seq_t)1;               /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t ii = (seq_t)i;             /* cast index to seq_t */
        seq_t g  = gcd(prev, ii);        /* gcd(a(n‑1), n) */
        prev = (prev / g) + (ii / g);    /* (a+ n)/g */
    }
    return prev;
}
