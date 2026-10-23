/*
 * A046635  Number of cubic residues mod 7^n.
 * https://oeis.org/A046635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) + a(n-3) - 7*a(n-4) (n >= 4), with a(0)=1,
 *            a(1)=3, a(2)=15, a(3)=99
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;     /* array of computed values */
static size_t cache_cap = 0;    /* allocated capacity */
static size_t cache_len = 0;    /* number of values currently computed */

/* Compute the n-th term of A046635 */
seq_t a(idx_t n)
{
    size_t idx = (size_t)n;

    /* Base cases for n < 4 – return immediately */
    if (idx == 0u) return (seq_t)1;
    if (idx == 1u) return (seq_t)3;
    if (idx == 2u) return (seq_t)15;
    if (idx == 3u) return (seq_t)99;

    /* Initialise cache on first call (or when size changes) */
    if (cache == NULL) {
        /* Allocate sufficient capacity for the requested index */
        cache_cap = (idx > 4 ? idx + 1 : 4);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;      /* allocation failed */
        /* Store the first four values */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)15;
        cache[3] = (seq_t)99;
        cache_len = 4;   /* we have computed indices 0..3 */
    }

    /* Resize the cache if necessary */
    if (idx + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= idx) {
            /* Grow linearly until we have enough space */
            new_cap = new_cap * 2 + 1;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;  /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing values up to idx */
    if (idx >= cache_len) {
        for (size_t i = cache_len; i <= idx; ++i) {
            seq_t val = 7 * cache[i - 1];
            val += cache[i - 3];
            seq_t sub = 7 * cache[i - 4];
            val -= sub;
            cache[i] = val;
        }
        cache_len = idx + 1;
    }

    return cache[idx];
}
