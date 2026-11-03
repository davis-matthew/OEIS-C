/*
 * A079431  Least m>n with same smallest odd prime factor as n.
 * https://oeis.org/A079431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9688 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute the least m > n having the same smallest odd prime factor
    as n.  The routine is a straightforward search that checks each
    candidate integer using the utility function
    smallest_odd_prime_factor(uint64_t v).  For powers of two the
    smallest odd prime factor is defined to be 1, so the next term is
    simply the next power of two.  This implementation does not rely
    on any precomputed tables and works for any idx_t value.
*/
#include <stdint.h>

static inline uint64_t smallest_odd_prime_factor(uint64_t v)
{
    if (v <= 2)              /* 1 or 2 → no odd prime divisor */
        return 0;
    /* Remove all factors of 2 */
    while ((v & 1ULL) == 0)  /* v % 2 == 0 */
        v >>= 1;             /* v /= 2 */
    if (v == 1)              /* only factor 2 remained */
        return 0;

    /* Find the smallest odd divisor of the remaining odd part */
    for (uint64_t d = 3; d * d <= v; d += 2) {
        if (v % d == 0)
            return d;        /* first odd divisor found is the smallest odd prime factor */
    }
    return v;                /* v itself is prime and odd */
}

seq_t a(idx_t n)
{
    /* Determine the smallest odd prime factor of n (1 if n is a power
       of 2). */
    uint64_t p = smallest_odd_prime_factor((uint64_t)n);

    /* Powers of two (p == 1) are handled specially: the next term is
       the next power of two. */
    if (p == 1) {
        /* n itself must be a power of two; return the next one. */
        return (seq_t)(n << 1);
    }

    /* For other n, search forward for the first integer having the
       same smallest odd prime factor. */
    idx_t m = n + 1;
    for (;;) {
        if (smallest_odd_prime_factor((uint64_t)m) == p)
            return (seq_t)m;
        ++m;
    }
}
