/*
 * A063658  The number of integers m in [1..n] for which gcd(m,n) is divisible
 *          by a square greater than 1.
 * https://oeis.org/A063658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integers m (1 ≤ m ≤ n) for which gcd(m,n) is
 *            divisible by a square > 1. This is equivalent to a(n) = Σ_{d|n,
 *            d not square‑free} φ(n/d); the implementation below counts
 *            directly by testing each m
 * Range:     u8 n <= 791, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd_u64.h"

/* Compute gcd of two 64‑bit unsigned integers. */

/* Return true if x is square‑free, i.e. no prime divides x twice.  */
static inline bool is_squarefree(idx_t x)
{
    for (idx_t p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            x /= p;                /* remove one factor of p */
            if (x % p == 0)        /* a second factor => not square‑free */
                return false;
            while (x % p == 0)     /* strip all remaining factors of p */
                x /= p;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* sequence is 1‑based; undefined for 0 */
        return 0;

    seq_t cnt = 0;

    for (idx_t m = 1; m <= n; ++m) {
        uint64_t g = gcd_u64(n, m);
        if (g > 1 && !is_squarefree(g))   /* gcd contains a square factor */
            ++cnt;
    }

    return cnt;
}
