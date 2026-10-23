/*
 * A160666  Numbers whose distance to the closest prime number is a prime
 *          number.
 * https://oeis.org/A160666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth non‑prime integer m such that the distance from m
 *            to the closest prime number is prime. a(1) = 0, a(2) = 9, a(3) =
 *            15, … (OEIS A160666)
 * Range:     u8 n <= 96, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

#ifdef __cplusplus
#include <vector>

static std::vector<seq_t> cache;
static idx_t current = 0;  // next integer to test
static idx_t found  = 0;  // how many qualifying numbers found

seq_t a(idx_t n) {
    if (n == 0) return 0;          // behaviour below offset is irrelevant
    while (found < n) {
        seq_t cand = (seq_t)current;

        /* distance to the nearest prime below */
        idx_t dist_down = (idx_t)-1;
        if (current > 0) {
            idx_t t = current;
            while (t > 0 && !is_prime(t)) t--;
            if (t > 0 && is_prime(t)) dist_down = current - t;
        }

        /* distance to the nearest prime above (or at) */
        idx_t t_up = current;
        while (!is_prime(t_up)) t_up++;
        idx_t dist_up = t_up - current;

        idx_t min_dist = (dist_down < dist_up) ? dist_down : dist_up;
        if (min_dist != (idx_t)-1 && is_prime(min_dist))
            cache.push_back(cand);

        if (!cache.empty() && cache.back() == cand) found++;

        current++;
    }
    return cache[n - 1];
}

#else  /* C implementation */

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t current = 0;  // next integer to test
static idx_t found  = 0;   // how many qualifying numbers found

static void expand_cache(size_t needed) {
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* behaviour below offset is irrelevant */
    while (found < n) {
        seq_t cand = (seq_t)current;

        /* distance to the nearest prime below */
        idx_t dist_down = (idx_t)-1;
        if (current > 0) {
            idx_t t = current;
            while (t > 0 && !is_prime(t)) t--;
            if (t > 0 && is_prime(t)) dist_down = current - t;
        }

        /* distance to the nearest prime above (or at) */
        idx_t t_up = current;
        while (!is_prime(t_up)) t_up++;
        idx_t dist_up = t_up - current;

        idx_t min_dist = (dist_down < dist_up) ? dist_down : dist_up;
        if (min_dist != (idx_t)-1 && is_prime(min_dist)) {
            expand_cache(cache_len + 1);
            cache[cache_len++] = cand;
            found++;
        }
        current++;
    }
    return cache[n - 1];
}

#endif
