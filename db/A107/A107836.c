/*
 * A107836  Slowest increasing sequence where a(n)+(first digit of a(n)) is
 *          prime.
 * https://oeis.org/A107836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = next integer > a(n-1) such that a(n) + first_digit(a(n))
 *            is prime with a(1) = 1. The sequence is generated lazily and
 *            memoised so that multiple calls with arbitrary n are fast
 * Range:     u8 n <= 54, u16 n <= 6543, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9567 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

static seq_t *cache = NULL;      /* memoisation array */
static idx_t cache_len = 0;      /* number of terms stored */

/* Helper: return the first (most significant) digit of a non‑negative number */
static seq_t first_digit(seq_t x)
{
    seq_t d = x;
    while (d >= 10) d /= 10;
    return d;
}

seq_t a(idx_t n)
{
    /* The sequence is 1‑based; ignore n == 0 (undefined) */
    if (n == 0) return 0;

    /* Allocate or enlarge the cache as needed */
    if (n > cache_len) {
        size_t newcap = n;
        seq_t *tmp = (cache == NULL) ?
            mem_new(seq_t, newcap) :
            mem_resize(seq_t, cache, newcap);
        if (!tmp) return 0; /* allocation failure – return 0 (unused) */
        cache = tmp;

        seq_t prev = (cache_len > 0) ? cache[cache_len - 1] : 0;
        for (idx_t i = cache_len; i < n; ++i) {
            seq_t cand = prev + 1;
            /* Search for the next candidate satisfying the primality test */
            while (1) {
                seq_t fd = first_digit(cand);
                uint64_t sum = (uint64_t)cand + (uint64_t)fd;
                if (is_prime_u64(sum)) break;
                ++cand;
            }
            cache[i] = cand;
            prev = cand;
        }
        cache_len = n;
    }
    return cache[n - 1];
}
