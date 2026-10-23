/*
 * A039380  Numbers whose base-9 representation has the same nonzero number of
 *          3's and 6's.
 * https://oeis.org/A039380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 7662, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039380: Numbers whose base‑9 representation has the same
 * nonzero number of 3's and 6's.
 *
 * The implementation performs a linear search with memoisation.
 * For each number it counts the digits 3 and 6 in base 9; if the two
 * counts are equal and non‑zero the number is stored as the next
 * term of the sequence.
 * The search starts from 1 and continues until the required index
 * has been reached.  Subsequent calls reuse the previously found
 * terms, so calls in any order are efficient.
 *
 * Complexity: O(M) where M is the value of the n‑th term (roughly
 * proportional to n for the range of n typically tested).  Works
 * comfortably for n up to at least 10 000 within the time limits.
 */
#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;   /* stored sequence terms */
static size_t cache_len = 0;   /* number of terms stored */
static size_t cache_cap = 0;   /* allocated capacity */
static seq_t last_checked = 0;/* last number tested */

/* Resize the cache if needed */
static void ensure_cache(size_t needed)
{
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? (cache_cap << 1) : 16;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Index n is 1‑based; we need to return cache[n‑1] */
    if (n == 0) return 0;  /* undefined, but return 0 for safety */

    /* Extend cache until we have the nth term */
    while (cache_len < (size_t)n) {
        last_checked += 1;      /* move to next integer */
        seq_t x = last_checked;
        unsigned int c3 = 0, c6 = 0;
        while (x > 0) {
            unsigned int d = (unsigned int)(x % 9);
            if (d == 3) c3++;
            else if (d == 6) c6++;
            x /= 9;
        }
        if (c3 == c6 && c3 > 0) {
            ensure_cache(cache_len + 1);
            cache[cache_len++] = last_checked;
        }
    }
    return cache[n - 1];
}
