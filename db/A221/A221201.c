/*
 * A221201  Number of 2 X n arrays of occupancy after each element moves to
 *          some horizontal, diagonal or antidiagonal neighbor, with no
 *          occupancy greater than 2.
 * https://oeis.org/A221201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Formula (recurrence):
    a(1)=0, a(2)=9, a(3)=10, a(4)=196, a(5)=720, a(6)=6400,
    a(7)=34272, a(8)=242064, a(9)=1460368, ...
    a(n) = 7*a(n-1) + 14*a(n-2) - 126*a(n-3) + 504*a(n-5)
           - 224*a(n-6) - 448*a(n-7) + 256*a(n-8)    for n ≥ 9
*/

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache[0..len-1] holds a(0)..a(len-1) */
static idx_t   cache_len = 0;    /* number of elements stored */

/* Initial values 1..9 (a(0) unused, a(1)=0) */
static const seq_t base_vals[10] = {
    0ULL,            /* index 0 unused */
    0ULL,            /* a(1) */
    9ULL,            /* a(2) */
    10ULL,           /* a(3) */
    196ULL,          /* a(4) */
    720ULL,          /* a(5) */
    6400ULL,         /* a(6) */
    34272ULL,        /* a(7) */
    242064ULL,       /* a(8) */
    1460368ULL       /* a(9) */
};

/* Ensure capacity for indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (cache_len > n) return;          /* already large enough */

    size_t new_len = (size_t)(n + 1);    /* indices 0..n */

    seq_t *new_cache = (seq_t*)malloc(new_len * sizeof(seq_t));
    if (!new_cache) return;              /* allocation failed – bail */

    /* copy existing data */
    if (cache) {
        for (idx_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }
    /* initialise new slots to zero */
    for (idx_t i = cache_len; (idx_t)i < new_len; ++i)
        new_cache[i] = 0;

    cache     = new_cache;
    cache_len = (idx_t)new_len;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;               /* out of range, harmless */

    idx_t prev_len = cache_len;                /* compute from this index */
    ensure_cache(n);
    if (prev_len == 0) prev_len = 1;           /* skip unused index 0 */

    for (idx_t k = prev_len; k <= n; ++k) {
        if (k <= 9) {                          /* initialise first 9 terms */
            cache[k] = base_vals[(size_t)k];
        } else {                               /* recurrence for k ≥ 10 */
            seq_t val = 0;
            val += 7ULL   * cache[k - 1];
            val += 14ULL  * cache[k - 2];
            val -= 126ULL * cache[k - 3];
            val += 504ULL * cache[k - 5];
            val -= 224ULL * cache[k - 6];
            val -= 448ULL * cache[k - 7];
            val += 256ULL * cache[k - 8];
            cache[k] = val;
        }
    }

    return cache[n];
}
