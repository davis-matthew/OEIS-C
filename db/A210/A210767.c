/*
 * A210767  Numbers whose digit sum as well as sum of the 4th powers of the
 *          digits is a prime.
 * https://oeis.org/A210767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth positive integer whose digit sum and sum of
 *            fourth powers of its decimal digits are both prime
 * Range:     u8 n <= 53, u16 n <= 3373, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Precomputed fourth powers of digits 0-9 */
static const uint64_t pow4_digit[10] = {
    0ULL, 1ULL, 16ULL, 81ULL, 256ULL, 625ULL,
    1296ULL, 2401ULL, 4096ULL, 6561ULL
};

/* Small prime test (works for values <= 10^9, which is sufficient here). */
static bool is_prime_small(uint64_t n) {
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (uint64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

/* Compute digit sum and sum of fourth powers of digits for a uint64_t. */
static void digit_sums_idx(idx_t v, uint64_t *sd, uint64_t *sf) {
    uint64_t sum_digits = 0, sum_fourth = 0;
    while (v > 0) {
        uint64_t d = v % 10;
        sum_digits += d;
        sum_fourth += pow4_digit[d];
        v /= 10;
    }
    *sd = sum_digits;
    *sf = sum_fourth;
}

/* Memoization cache for the sequence. */
static seq_t *cache = NULL;          /* array of values */
static idx_t  cache_used = 0;        /* number of values stored */
static idx_t  current     = 0;        /* last tested integer */

/* Generate sequence terms up to target index (1‑based). */
static void ensure_cache(idx_t target) {
    /* Reserve space for new terms. */
    while (cache_used < target) {
        current++;
        uint64_t sd, sf;
        digit_sums_idx(current, &sd, &sf);
        if (is_prime_small(sd) && is_prime_small(sf)) {
            /* Append to cache. */
            seq_t *new_cache = (seq_t *)realloc(cache,
                                                (cache_used + 1) * sizeof(seq_t));
            /* If realloc failed, abort to avoid undefined behavior. */
            if (!new_cache) abort();
            cache = new_cache;
            cache[cache_used] = (seq_t)current;
            cache_used++;
        }
    }
}

/* Public function required by the build. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;      /* undefined for n<1, but safe guard */
    /* Compute terms up to n if necessary. */
    if (cache_used < n) ensure_cache(n);
    return cache[n - 1];
}
