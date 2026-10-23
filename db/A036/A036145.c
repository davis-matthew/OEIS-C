/*
 * A036145  3^n mod 137.
 * https://oeis.org/A036145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n mod 137
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* We only need 64‑bit arithmetic for the intermediate products
       because 137^2 = 18769 < 2^16. */
    const uint64_t mod   = 137ull;
    uint64_t      base = 3ull % mod;
    uint64_t      exp  = n;
    uint64_t      res  = 1ull % mod;

    while (exp > 0) {
        if (exp & 1ull)
            res = (res * base) % mod;
        base = (base * base) % mod;
        exp >>= 1ull;
    }

    return (seq_t)res;
}
