/*
 * A081259  a(n) is the smallest k such that C(3n,n) divides k!.
 * https://oeis.org/A081259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime < 3n
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the smallest k such that C(3n,n) divides k!, which equals
   the largest prime less than 3n. */

#include <stdint.h>

/* Simple trial‑division primality test (sufficient for 3n up to ~10⁶). */
static int is_prime_uint64(uint64_t n)
{
    if (n < 2) return 0;
    if (n == 2) return 1;
    if ((n & 1) == 0) return 0;          /* even numbers >2 are composite */
    for (uint64_t d = 3; d <= n / d; d += 2) {
        if (n % d == 0) return 0;
    }
    return 1;
}

seq_t a(idx_t n)
{
    uint64_t m = 3ULL * n;          /* 3n may overflow for very large n but
                                       n is at most 64‑bit; the product fits
                                       in 64 bits for all practical n in the
                                       test harness. */
    uint64_t x = m > 0 ? m - 1 : 0;  /* start from the largest integer <3n */

    if (x < 2) return (seq_t)2;     /* handles n = 1 → a(1) = 2 */

    /* Ensure we test odd candidates first (except 2). */
    if (x > 2 && (x & 1) == 0) x--;

    for (; x >= 2; x -= 2 + ((x == 2) ? 0 : 0)) /* decrement by 2, but keep
                                                 2 as a special case */
    {
        if (x == 2) { return (seq_t)2; }
        if (is_prime_uint64(x)) return (seq_t)x;
    }

    /* Fallback – should not be reached for n ≥ 1. */
    return (seq_t)2;
}
