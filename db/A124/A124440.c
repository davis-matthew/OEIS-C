/*
 * A124440  a(n) = Sum_{n/2<=k<=n, gcd(k,n)=1} k.
 * https://oeis.org/A124440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=ceil(n/2)}^n [gcd(k,n)=1] * k
 * Range:     u8 n <= 28, u16 n <= 418, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    if (n == 0) return 0;  /* for safety, though offset starts at 1 */

    /* compute ceil(n/2) */
    idx_t start = n / 2;
    if (n % 2 != 0) start++;

    for (idx_t k = start; k <= n; ++k)
    {
        if (gcd((seq_t)k, (seq_t)n) == (seq_t)1)
            sum += k;
    }

    return sum;
}
