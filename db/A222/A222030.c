/*
 * A222030  Primes and quarter-squares.
 * https://oeis.org/A222030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 63 (2026-11-01)
 * Helpers:   ipow_i32, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A222030: Union of primes and quarter‑squares (floor(m^2/4)) */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/is_prime.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

#define CACHE_SIZE 200000   /* enough for n up to ~10 000 */

seq_t a(idx_t n) {
    static seq_t cache[CACHE_SIZE];
    static size_t cache_len = 0;
    static seq_t cand = 0;   /* next integer to test */

    if (n < cache_len) return cache[n];

    while (cache_len <= n) {
        bool present = false;

        /* check primality */
        if (is_prime((idx_t)cand)) present = true;

        /* check quarter‑square: either a square or a pronic number */
        if (!present) {
            seq_t sq = floor_sqrt(cand);
            if (sq * sq == cand) present = true;
            else if (sq * (sq + 1) == cand) present = true;
        }

        /* store if present */
        if (present) {
            cache[cache_len++] = cand;
        }

        cand++;
    }

    return cache[n];
}
