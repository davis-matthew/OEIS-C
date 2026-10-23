/*
 * A118950  Numbers containing at least one prime digit.
 * https://oeis.org/A118950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 184, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation:  scan integers for a prime digit (2,3,5,7), memoize results */
/* Returning the n-th positive integer that contains at least one prime digit. */

#include <stdlib.h>
#include <stdbool.h>

/* local helper: test if the input has at least one digit in {2,3,5,7} */
static inline bool has_prime_digit(seq_t x) {
    while (x != 0) {
        unsigned int d = (unsigned int)(x % 10);
        if (d == 2 || d == 3 || d == 5 || d == 7) return true;
        x /= 10;
    }
    return false;
}

/* dynamic cache for already computed a(k) values */
static seq_t *cache  = NULL;
static size_t  cap   = 0;
static size_t  sz    = 0;

/* grow cache capacity to at least 'needed' */
static void ensure_capacity(size_t needed) {
    if (cap >= needed) return;
    size_t new_cap = cap ? cap * 2 : 16;
    if (new_cap < needed) new_cap = needed;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* allocation failed; terminate program */
        exit(1);
    }
    cache = new_cache;
    cap   = new_cap;
}

/* main function requested by the build */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* not used, but safe */
    while (sz < (size_t)n) {
        seq_t cand = (sz == 0) ? 2 : cache[sz - 1] + 1;   /* start searching after last found */
        while (!has_prime_digit(cand)) {
            cand += 1;
        }
        ensure_capacity(sz + 1);
        cache[sz++] = cand;
    }
    return cache[n - 1];
}
