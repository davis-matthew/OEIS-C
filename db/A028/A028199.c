/*
 * A028199  Expansion of 1/((1-5x)(1-10x)(1-11x)(1-12x)).
 * https://oeis.org/A028199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 23*a(n-1) - 132*a(n-2) + 5^n * (2^(n+1)-1) for n >= 2
 *            a(0) = 1, a(1) = 38
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses a simple O(n) memoised recurrence. */
#include <stdlib.h>

static seq_t *cache       = NULL;   /* array holding computed terms   */
static size_t cache_cap   = 0;      /* current allocated capacity     */
static size_t cache_len   = 0;      /* number of terms stored         */
static seq_t pow5         = 0;      /* 5^i for the last cached i     */
static seq_t pow2plus1    = 0;      /* 2^(i+1) for the last cached i */
static int   initialized  = 0;      /* 0 = not yet initialised       */

seq_t a(idx_t n)
{
    /* trivial cases handled directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)38;

    /* initialise static data on first use */
    if (!initialized) {
        cache_cap = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)38;
        cache_len = 2;
        pow5      = (seq_t)5;   /* 5^1 for index 1                  */
        pow2plus1 = (seq_t)4;   /* 2^(1+1) = 4 for index 1          */
        initialized = 1;
    }

    /* grow the cache up to the requested index */
    while (cache_len <= (size_t)n) {
        /* double capacity if necessary */
        if (cache_len >= cache_cap) {
            cache_cap = (cache_cap == 0 ? 4 : cache_cap * 2);
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
            if (!tmp) exit(EXIT_FAILURE);
            cache = tmp;
        }

        /* compute 5^(i+1) and 2^((i+1)+1) based on the last cached i */
        seq_t next_pow5      = pow5 * (seq_t)5;
        seq_t next_pow2plus1 = pow2plus1 * (seq_t)2;

        /* term = 5^(i+1) * (2^((i+1)+1) - 1) */
        seq_t term = next_pow5 * (next_pow2plus1 - (seq_t)1);

        /* recurrence: a(i+1) = 23*a(i) - 132*a(i-1) + term */
        seq_t val = (seq_t)23 * cache[cache_len - 1]
                  - (seq_t)132 * cache[cache_len - 2]
                  + term;

        cache[cache_len++] = val;
        pow5      = next_pow5;
        pow2plus1 = next_pow2plus1;
    }

    return cache[(size_t)n];
}
