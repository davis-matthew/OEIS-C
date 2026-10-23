/*
 * A128968  a(n) is the n-th prime of the form x^2+n.
 * https://oeis.org/A128968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th prime of the form x^2 + n, offset 1
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 13, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for offset 1, safe guard */

    idx_t count = 0;                 /* how many primes found so far */
    idx_t x = 0;                     /* current x value */

    for (;;) {
        seq_t p = (seq_t)x * (seq_t)x + (seq_t)n;   /* candidate prime */
        if (is_prime_big(p)) {
            ++count;
            if (count == n) return p;
        }
        ++x;
    }

    /* unreachable, keeps compilers happy */
    return 0;
}
