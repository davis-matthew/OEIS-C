/*
 * A116982  Distance between prime neighbors of 4n.
 * https://oeis.org/A116982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime_next(4*n) - prime_prev(4*n)
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Validated: partial, pass, 93 terms through n = 93 (2026-11-01)
 * Helpers:   is_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* distance between the prime neighbors of 4n */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/next_prime.h"

/* Return the largest prime < n   (n > 2)   ; pp(2) = 1, but we never call it for n==2 */
static inline idx_t prev_prime(idx_t n)
{
    idx_t p = n - 1;
    for (;; --p)
    {
        if (is_prime(p)) return p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* no definition for n = 0, return 0 */
    idx_t m   = 4 * n;       /* 4*n */
    idx_t prev = prev_prime(m);
    idx_t next = next_prime(m);
    return (seq_t)(next - prev);
}
