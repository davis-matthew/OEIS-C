/*
 * A216094  a(1) = 2; a(n+1) = a(n) + |A036263(n)| + 2.
 * https://oeis.org/A216094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 2; a(n+1) = a(n) + |prime(n+2) - 2*prime(n+1) + prime(n)|
 *            + 2;
 * Range:     u8 n <= 49, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Sequence is defined for n >= 1. For n < 1 we return 0 (undefined). */
    if (n == 0)
        return 0;

    /* a(1) = 2 */
    seq_t cur = 2;
    if (n == 1)
        return cur;

    /* Compute successive terms up to a(n) */
    for (idx_t i = 1; i < n; ++i) {
        /* primes are 1‑based: p(i) = prime(i) */
        uint32_t p   = get_prime(i);
        uint32_t p1  = get_prime(i + 1);
        uint32_t p2  = get_prime(i + 2);

        /* |p(i+2) - 2*p(i+1) + p(i)| as an integer */
        int64_t diff_signed = (int64_t)p2 - 2LL * (int64_t)p1 + (int64_t)p;
        seq_t diff = diff_signed >= 0 ? (seq_t)diff_signed : (seq_t)(-diff_signed);

        /* a(i+1) = a(i) + diff + 2 */
        cur = cur + diff + (seq_t)2;
    }

    return cur;
}
