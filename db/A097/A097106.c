/*
 * A097106  a(n) = (Smallest prime power >= n) - (greatest prime power <= n).
 * https://oeis.org/A097106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = (smallest prime power >= n) - (greatest prime power <= n)
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9316 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 0; a(n) = 0 iff n itself is a (ordinary) prime power. */

#include <stdbool.h>
#include <stdint.h>

/* Test whether an unsigned 64‑bit integer is a prime power.
   Return 0 for <2 to keep 1 from being treated as a prime power.  */
static inline bool is_prime_power_uint64(uint64_t v)
{
    if (v < 2) return false;
    for (uint64_t p = 2; p * p <= v; ++p) {
        if (v % p == 0) {
            while (v % p == 0) v /= p;
            return v == 1;
        }
    }
    /* v is prime */
    return true;
}

/* Main entry point for the OEIS sequence.  */
seq_t a(idx_t n)
{
    if (n == 1) return 0;          /* a(1) is defined as 0 */

    idx_t low = n;
    while (low >= 2 && !is_prime_power_uint64((uint64_t)low))
        --low;                     /* lower bound, at least 2 */

    idx_t high = n;
    while (!is_prime_power_uint64((uint64_t)high))
        ++high;                    /* upper bound */

    return (seq_t)(high - low);
}
