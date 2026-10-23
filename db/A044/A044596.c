/*
 * A044596  Numbers n such that string 3,4 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044596: numbers n such that the base‑8 representation of n contains
 * the substring "34" but the base‑8 representation of n+1 does not.
 * The function computes the n‑th such number by searching sequentially
 * and caching results for later calls. */

#include <stdbool.h>
#include <stdlib.h>

/* Static cache for found terms */
static seq_t *cache = NULL;      /* array of seq_t */
static idx_t  cache_len = 0;     /* number of cached terms */
static idx_t  cache_cap = 0;     /* allocated capacity of cache array */
static seq_t  next_candidate = 0;/* next integer to examine */

/* Helper: test whether the base‑8 representation of x contains "34". */
static bool has_34(seq_t x)
{
    if (x == 0) return false;           /* "0" does not contain "34" */

    enum { DIGITS_MAX = 256 };
    uint8_t digits[DIGITS_MAX];
    unsigned len = 0;

    /* Extract base‑8 digits (least significant first) */
    seq_t tmp = x;
    while (tmp != 0 && len < DIGITS_MAX) {
        digits[len++] = (uint8_t)(tmp % 8);
        tmp /= 8;
    }

    /* Scan from most significant toward least significant digit. */
    for (int i = (int)len - 1; i > 0; --i) {
        if (digits[i] == 3 && digits[i - 1] == 4) {
            return true;
        }
    }
    return false;
}

/* Public function: returns the n-th term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset 1 */

    /* Extend cache until we have at least n terms. */
    while (cache_len < n) {
        /* Ensure enough capacity in cache array. */
        if (cache_len == cache_cap) {
            idx_t new_cap = cache_cap ? cache_cap * 2 : 128;
            seq_t *new_cache = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
            if (!new_cache) abort();  /* out of memory */
            cache = new_cache;
            cache_cap = new_cap;
        }

        /* Test whether next_candidate satisfies the condition. */
        if (has_34(next_candidate) && !has_34(next_candidate + 1)) {
            cache[cache_len++] = next_candidate;
        }
        next_candidate++;
    }

    /* Return the stored term (offset 1 => index n-1). */
    return cache[n - 1];
}
