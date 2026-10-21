/*
 * A214737  Least m>0 such that n-m divides prime(n)+m.
 * https://oeis.org/A214737
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least m>0 such that (n-m) | (prime(n)+m)
 * Range:     u8 n <= 47, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                 /* undefined for n < 2 */

    uint32_t p32 = get_prime(n);  /* n-th prime (1‑based indexing) */
    seq_t p = (seq_t)p32;

    for (idx_t m = 1; m < n; ++m) {
        idx_t d_idx = n - m;
        seq_t d = (seq_t)d_idx;
        seq_t val = p + (seq_t)m;
        if ((val % d) == 0)
            return (seq_t)m;
    }

    return (seq_t)(n - 1);        /* fallback – occurs only if no divisor found */
}
