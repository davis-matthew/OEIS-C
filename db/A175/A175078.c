/*
 * A175078  Number of iterations of {r mod (max prime p < r)} needed to reach
 *          1 or 2 starting at r = n.
 * https://oeis.org/A175078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of iterations of r = r % (max prime < r) until r ∈
 *            {1, 2}
 * Range:     u8 n <= 1035, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 116 terms through n = 9307 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* Return the largest prime < n   (n > 2)   ; pp(2) = 1, but we never call it for n==2 */
static inline idx_t prev_prime(idx_t n)
{
    idx_t p = n - 1;
    for (;; --p)
    {
        if (is_prime(p)) return p;
    }
}
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 2)
        return 0;

    seq_t r = n;
    seq_t steps = 0;

    while (r > 2) {
        uint64_t r_u = (uint64_t)r;        /* r fits in uint64_t */
        uint64_t p  = prev_prime(r_u);     /* largest prime < r */
        r = r % (seq_t)p;
        steps++;
    }

    return steps;
}
