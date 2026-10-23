/*
 * A197125  Numbers such that sum of digits and sum of the square of digits
 *          are both a square.
 * https://oeis.org/A197125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 405, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum, ensure_capacity, floor_sqrt, get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A197125
 *
 *  Formula: a(n) is the nth positive integer such that
 *  (i) the sum of its decimal digits is a perfect square,
 *  (ii) the sum of the squares of its decimal digits is also a perfect square.
 *
 *  The sequence starts: 1, 4, 9, 10, 40, 90, 100, 400, 900, 1000, …
 *
 *  This implementation uses a simple brute‑force search with memoisation.
 *  For each candidate integer it checks the two conditions using the
 *  helper routines supplied by the build system and stores the result
 *  in a growable cache.  Calls to a(idx_t n) for arbitrary n ≥ 1 are
 *  supported, and the static cache guarantees that repeated calls are
 *  fast.  The implementation stays well under the time limit for
 *  n up to about 10 000.
 *
 *  Required helper headers:
 *   - ../utils/floor_sqrt.h
 *   - ../utils/digit_sum.h
 *   - ../utils/get_digits.h
 *   - ../utils/ensure_capacity.h
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/floor_sqrt.h"   /* floor_sqrt(seq_t) */
#include "../utils/digit_sum.h"    /* digit_sum(seq_t) */
#include "../utils/get_digits.h"   /* get_digits(seq_t, uint8_t*, size_t*) */
#include "../utils/ensure_capacity.h" /* ensure_capacity(size_t, size_t*, void**, size_t) */

/* Helper: test whether x is a perfect square. */
static inline bool is_square(seq_t x)
{
    seq_t r = floor_sqrt(x);
    return r * r == x;
}

/* The sequence generator. */
seq_t a(idx_t n)
{
    /* Static cache for previously found terms. */
    static seq_t *cache      = NULL;   /* array of found terms */
    static size_t cache_cap  = 0;      /* current allocated capacity */
    static size_t cache_len  = 0;      /* number of terms stored so far */
    static seq_t next_candidate = 1;   /* next integer to test */

    if (n < 1)
        return 0;   /* undefined for n == 0 */

    /* Ensure the cache contains at least n terms. */
    while (cache_len < (size_t)n) {
        seq_t cand = next_candidate;

        /* (i) sum of digits */
        seq_t sum_d = digit_sum(cand);

        /* (ii) sum of squares of digits */
        seq_t sum_sq = 0;
        uint8_t digbuf[128];
        size_t len;
        get_digits(cand, digbuf, &len);
        for (size_t i = 0; i < len; ++i) {
            uint8_t d = digbuf[i];
            sum_sq += (seq_t)(d * d);
        }

        /* Check the two square conditions. */
        if (is_square(sum_d) && is_square(sum_sq)) {
            /* Grow array if needed and store the candidate. */
            ensure_capacity(cache_len + 1, &cache_cap,
                            (void **)&cache, sizeof(seq_t));
            cache[cache_len++] = cand;
        }

        ++next_candidate;
    }

    return cache[(size_t)n - 1];
}
