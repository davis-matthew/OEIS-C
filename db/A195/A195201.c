/*
 * A195201  a(n) is the largest prime factor of the number made by taking the
 *          previous term and duplicating the final digit, a(1) = 1.
 * https://oeis.org/A195201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   The sequence is defined by a(1) = 1 for n > 1: a(n) = largest
 *            prime factor of (a(n‑1) × 10 + (a(n‑1) mod 10))
 * Range:     u8 n <= 6, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return the largest prime factor of a positive 64‑bit integer.
 * For 1 the function returns 1. */
static uint64_t _gpf_u64(uint64_t n)
{
    if (n < 2)
        return n;

    uint64_t maxp = 1;

    /* factor out all 2s */
    while ((n & 1ULL) == 0) {
        maxp = 2;
        n >>= 1;
    }

    /* odd factors */
    for (uint64_t i = 3; i * i <= n; i += 2) {
        while (n % i == 0) {
            maxp = i;
            n /= i;
        }
    }

    /* remaining prime factor > 1 */
    if (n > 1)
        maxp = n;

    return maxp;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;                     /* a(1) = 1 */

    uint64_t cur = 1;                  /* current term, fits in 64 bits */

    /* iterate from 2 up to n */
    for (idx_t k = 2; k <= n; ++k)
    {
        uint64_t next = cur * 10ULL + (cur % 10ULL);
        cur = _gpf_u64(next);
    }

    return (seq_t)cur;
}
