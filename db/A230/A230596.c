/*
 * A230596  Number of ways to write n = x + y + z with 0 < x <= y <= z such
 *          that x*y*z is a triangular number, and that x is a triangular
 *          number of the form (p^2 - 1)/8 with p an odd prime.
 * https://oeis.org/A230596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   u128_to_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A230596
 *  a(n) = number of triples (x,y,z) such that
 *      0 < x <= y <= z,
 *      x + y + z = n,
 *      x = (p^2 - 1)/8 with p an odd prime,
 *      and x*y*z is a triangular number.
 *
 *  Algorithm: brute‑force over all admissible x and y.
 *  For each x = (p^2-1)/8 up to n, enumerate y from x to (n-x)/2
 *  (ensures y <= z).  For each triple test if 8·x·y·z+1 is a perfect
 *  square, i.e. if the product is triangular.
 *
 *  Complexity:  O(π(√(8n)) * n) ≈ O(n·log n) for n up to ~10⁴,
 *  fast enough for the required limits.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/u128_to_u64.h"

/* integer square root of a 64‑bit unsigned integer */
static inline uint64_t isqrt_u64(uint64_t x)
{
    uint64_t r = 0;
    uint64_t bit = (uint64_t)1 << 62;            /* highest power of 4 <= x */
    while (bit > x) bit >>= 2;
    while (bit) {
        uint64_t temp = r + bit;
        if (x >= temp) {
            x -= temp;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* simple primality test for odd p (p <= 1000 is enough for n<=10⁴) */
static inline bool is_odd_prime(uint64_t p)
{
    if (p <= 2) return false;
    if (p % 2 == 0) return false;
    uint64_t lim = isqrt_u64(p);
    for (uint64_t q = 3; q <= lim; q += 2)
        if (p % q == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                  /* offset starts at 1 */

    seq_t count = 0;
    uint64_t un    = (uint64_t)n;
    uint64_t maxP  = isqrt_u64(8ULL * un + 1ULL);  /* largest p with (p²-1)/8 <= n */

    for (uint64_t p = 3; p <= maxP; p += 2) {      /* odd primes only */
        if (!is_odd_prime(p)) continue;

        uint64_t x = (p * p - 1U) / 8U;            /* x > 0 automatically */
        if (x > un) break;
        if (x == 0) continue;                     /* safety, though impossible */

        uint64_t rem = un - x;                    /* y + z */
        if (rem < 2 * x) continue;                /* minimal y=x gives z>=x */

        uint64_t y_min = x;
        uint64_t y_max = rem / 2;                 /* floor((n-x)/2) ensuring y <= z */

        for (uint64_t y = y_min; y <= y_max; ++y) {
            uint64_t z = rem - y;
            if (z < y) continue;                  /* safeguard */

            /* check triangularity of product */
            unsigned __int128 prod = (unsigned __int128)x * y * z;
            uint64_t t = u128_to_u64(prod * 8 + 1);    /* 8*x*y*z + 1 */
            uint64_t s = isqrt_u64(t);
            if (s * (unsigned __int128)s == t) {
                ++count;
            }
        }
    }
    return count;
}
