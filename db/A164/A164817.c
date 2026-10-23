/*
 * A164817  Positive numbers n with property that average digit of n^2 is an
 *          integer.
 * https://oeis.org/A164817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the n-th positive integer whose square has an integer
 *            average digit, i.e. the sum of the digits of n² is divisible by
 *            the number of digits of n². This implementation searches for
 *            candidates incrementally and caches the results so that any
 *            order of calls to a() is fast
 * Range:     u8 n <= 6, u16 n <= 36, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

#ifdef __cplusplus
#include <vector>
#endif

/* Static storage for the cache and the next candidate to test. */
static seq_t *cache_p = NULL;        /* Malloc‑based cache for C          */
static size_t cache_cap = 0;
static size_t cache_cnt = 0;
static seq_t cache_next = 1;          /* next integer to test             */

#ifdef __cplusplus
/* In C++ we use std::vector which handles construction/destruction
   of arbitrary‑precision types correctly.                                  */
static std::vector<seq_t> cache_vec;
static seq_t vec_next = 1;
#endif

/* Expand the C malloc‑based cache if necessary. */
static void ensure_cache(void) {
    if (cache_cnt == cache_cap) {
        size_t new_cap = cache_cap == 0 ? 16 : cache_cap << 1;
        seq_t *tmp = (seq_t *)realloc(cache_p, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache_p   = tmp;
        cache_cap = new_cap;
    }
}

/* Return the n‑th term of sequence A164817 (1‑based). */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined for index 0; safe return */

#ifdef __cplusplus
    /* C++ implementation using std::vector. */
    while (cache_vec.size() < (size_t)n) {
        seq_t sq = vec_next * vec_next;

        /* Compute digit sum and digit count of sq. */
        seq_t  sum   = 0;
        seq_t  tmp   = sq;
        size_t digits = 0;
        if (tmp == 0) {
            sum = 0;
            digits = 1;
        } else {
            while (tmp > 0) {
                sum    += tmp % (seq_t)10;
                tmp    /= (seq_t)10;
                ++digits;
            }
        }

        /* Check if the average digit is an integer. */
        if (sum % (seq_t)digits == 0) {
            cache_vec.push_back(vec_next);
        }
        ++vec_next;
    }
    return cache_vec[(size_t)n - 1];

#else
    /* C implementation using malloc/realloc. */
    while (cache_cnt < (size_t)n) {
        seq_t sq = cache_next * cache_next;

        /* Compute digit sum and digit count of sq. */
        seq_t  sum   = 0;
        seq_t  tmp   = sq;
        size_t digits = 0;
        if (tmp == 0) {
            sum = 0;
            digits = 1;
        } else {
            while (tmp > 0) {
                sum    += tmp % (seq_t)10;
                tmp    /= (seq_t)10;
                ++digits;
            }
        }

        /* Check if the average digit is an integer. */
        if (sum % (seq_t)digits == 0) {
            ensure_cache();
            cache_p[cache_cnt++] = cache_next;
        }
        ++cache_next;
    }
    return cache_p[(size_t)n - 1];
#endif
}
