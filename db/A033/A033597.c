/*
 * A033597  (nextprime(n)+prevprime(n))/2.
 * https://oeis.org/A033597
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (nextprime(n)+prevprime(n))/2
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 60 (2026-11-01)
 * Helpers:   is_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    /* The OEIS offset is 4: a(4) is the first term given. */
    if (n < 4)
        return 0;   /* value undefined below the offset */

    /* prev_prime(n) returns the largest prime < n. */
    idx_t pprev = prev_prime(n);

    /* next_prime(start) returns the smallest prime > start. */
    uint64_t pnext = next_prime((uint64_t)n);

    seq_t sprev = (seq_t)pprev;
    seq_t snext = (seq_t)pnext;

    return (sprev + snext) / 2;
}
