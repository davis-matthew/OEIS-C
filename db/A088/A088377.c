/*
 * A088377  a(n) = (smallest prime factor of n)^2; a(1) = 1.
 * https://oeis.org/A088377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (smallest prime factor of n)^2; a(1)=1
 * Range:     u8 n <= 16, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Validated: partial, pass, 76 terms through n = 76 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return the smallest prime factor of n (n >= 2).  For n = 1, we treat the
   result as 1 which ensures a(1)=1.  Since n is a 64‑bit unsigned integer,
   a simple trial division is sufficient for the required range. */
static inline uint64_t spf_uint64(uint64_t n)
{
    if (n % 2ULL == 0ULL)
        return 2ULL;
    for (uint64_t d = 3ULL; d * d <= n; d += 2ULL)
        if (n % d == 0ULL)
            return d;
    return n; /* n is prime */
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;                /* a(1)=1 and a(0) is unused */
    uint64_t p = spf_uint64((uint64_t)n);
    seq_t sp = (seq_t)p;                 /* promote to result type */
    return sp * sp;                      /* square of the smallest prime factor */
}
