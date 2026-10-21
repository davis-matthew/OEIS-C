/*
 * A078312  Greatest prime factor of n*rad(n)+1, where rad = A007947
 *          (squarefree kernel).
 * https://oeis.org/A078312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = greatest prime factor of n*rad(n)+1, where rad(n) =
 *            product of the distinct prime factors of n
 * Range:     u8 n <= 25, u16 n <= 313, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   local_rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/local_rad.h"

/* Greatest prime factor of a 64‑bit integer. */
static inline uint64_t gpf_uint64(uint64_t x)
{
    uint64_t maxp = 1;

    if (x % 2 == 0) {
        maxp = 2;
        while (x % 2 == 0) x /= 2;
    }
    for (uint64_t p = 3; p * p <= x; p += 2) {
        if (x % p == 0) {
            maxp = p;
            while (x % p == 0) x /= p;
        }
    }
    if (x > 1) maxp = x;      /* remaining prime factor */
    return maxp;
}

seq_t a(idx_t n)
{
    uint64_t rad_n = local_rad((uint64_t)n);

    /* Compute m = n * rad(n) + 1 safely in 128‑bit, then cast to 64‑bit. */
    unsigned __int128 prod = (unsigned __int128)n * rad_n + 1;
    uint64_t m = (uint64_t)prod;                /* fits for the tested range (≤10⁸) */

    uint64_t gp = gpf_uint64(m);
    return (seq_t)gp;
}
