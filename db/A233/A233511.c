/*
 * A233511  Replace the largest prime factor p>2 in n (if any) with the prime
 *          preceding p.
 * https://oeis.org/A233511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = replace the largest prime factor p>2 in n with the prime
 *            preceding p; if no prime factor or the largest one is 2, a(n) =
 *            n
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 91 terms through n = 9867 (2026-11-01)
 * Helpers:   is_prime, largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/largest_prime_factor.h"

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
    /* trivial cases */
    if (n <= 2)
        return n;

    /* n fits in uint64_t, same as idx_t */
    uint64_t nn = (uint64_t)n;

    /* largest prime factor of n */
    uint64_t lp = largest_prime_factor(nn);
    if (lp <= 2)          /* no replacement needed */
        return n;

    /* preceding prime of the largest prime factor */
    uint64_t pr = prev_prime(lp);

    /* replace one occurrence of the largest prime factor */
    seq_t res = (seq_t)(nn / lp) * (seq_t)pr;
    return res;
}
