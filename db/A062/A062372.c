/*
 * A062372  Numbers the product of whose nonzero digits is a cube.
 * https://oeis.org/A062372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th positive integer whose product of non‑zero
 *            decimal digits is a perfect cube (OEIS A062372)
 * Range:     u8 n <= 31, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   prod_nonzero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/prod_nonzero.h"

/* Integer floor cube root for a 64‑bit value.
   Uses binary search on the range [0, 2^21] because 2^21^3 = 2^63 > max 64‑bit. */
static inline uint64_t cube_root_floor(uint64_t n)
{
    uint64_t low = 0, high = 1 << 21;   /* 2^21 = 2097152, 2^21^3 > 2^63-1 */

    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        unsigned __int128 cube = (unsigned __int128)mid * mid * mid;
        if (cube <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Return the n-th term (n >= 1).  The implementation scans natural numbers
   and counts those whose digit‑product is a perfect cube. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not used, offset is 1 */

    idx_t count = 0;               /* number of found terms */
    uint64_t candidate = 1;        /* current integer to test */

    while (1) {
        uint64_t prod = prod_nonzero(candidate);
        uint64_t r = cube_root_floor(prod);
        if ((unsigned __int128)r * r * r == prod) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        ++candidate;
    }
}
