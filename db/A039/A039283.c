/*
 * A039283  Numbers whose base-5 representation has the same nonzero number of
 *          0's and 1's.
 * https://oeis.org/A039283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n‑th positive integer whose base‑5 representation
 *            contains the same positive number of digit 0’s and digit 1’s
 *            (both counts > 0)
 * Range:     u8 n <= 43, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* stored valid numbers in increasing order */
static size_t cache_cap = 0;       /* allocated capacity of cache */
static size_t cache_size = 0;      /* number of stored values */
static seq_t current = 0;          /* last candidate examined */

/* Return 1 if x has at least one 0 and at least one 1 and the numbers of
   0‑digits and 1‑digits are equal.  Both counts are integer numbers. */
static int is_valid(seq_t x)
{
    seq_t t = x;
    int cnt0 = 0, cnt1 = 0;
    if (t == 0) return 0;             /* base‑5 “0” has one zero but no ones   */
    while (t > 0) {
        unsigned digit = (unsigned)(t % (seq_t)5);
        if (digit == 0) ++cnt0;
        else if (digit == 1) ++cnt1;
        t /= (seq_t)5;
    }
    return cnt0 > 0 && cnt1 > 0 && cnt0 == cnt1;
}

/* Grow the cache so that it can hold at least new_cap elements. */
static void ensure_capacity(size_t new_cap)
{
    if (new_cap <= cache_cap) return;
    seq_t *tmp = NULL;
    if (cache == NULL) {
        tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    } else {
        tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    }
    if (tmp == NULL) {
        /* Out of memory – abort (behaviour undefined for the caller). */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, return 0 as a fallback */

    /* initialise the cache on the first call */
    if (cache == NULL) {
        cache_cap = 128;           /* start with a modest capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();
        cache_size = 0;
        current = 0;
    }

    /* extend the cache until we have at least n elements */
    while (cache_size < (size_t)n) {
        ++current;                 /* next candidate integer */
        if (is_valid(current)) {
            if (cache_size == cache_cap)
                ensure_capacity(cache_cap * 2);
            cache[cache_size++] = current;
        }
    }
    return cache[n - 1];
}
