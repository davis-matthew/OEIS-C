/*
 * A130231  a(1) = 3; a(n) = a(n-1) + (number of times the digit 3 has
 *          appeared in the sequence so far).
 * https://oeis.org/A130231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = a(n-1) + (number of times digit 3 has appeared in the
 *            sequence so far) a(1) = 3
 * Range:     u8 n <= 55, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_counts.h"

static seq_t *cache = NULL;
static size_t cached_len = 0;   /* number of elements actually stored */
static uint64_t total_3 = 0;    /* total count of digit '3' seen so far */

/* Extend the cache so that it contains terms a(1) … a(upto) */
static void extend_cache(idx_t upto)
{
    if (upto <= (idx_t)cached_len) return;  /* already computed */

    /* Grow the array */
    size_t new_len = (size_t)upto;
    seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) abort();  /* out of memory */
    cache = tmp;

    /* Generate the missing terms */
    idx_t start = (idx_t)cached_len;
    for (idx_t i = start; i < upto; ++i) {
        if (i == 0) {                     /* a(1) = 3 */
            cache[0] = (seq_t)3;
            unsigned char cnt[10];
            digit_counts(cache[0], cnt);
            total_3 = cnt[3];
        } else {
            seq_t prev = cache[i-1];
            seq_t val = prev + (seq_t)total_3;
            cache[i] = val;

            unsigned char cnt[10];
            digit_counts(val, cnt);
            total_3 += cnt[3];
        }
    }
    cached_len = new_len;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1, but harmless */
    extend_cache(n);
    return cache[n-1];
}
