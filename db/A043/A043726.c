/*
 * A043726  Numbers whose number of runs in the base-2 representation is
 *          congruent to 2 mod 4.
 * https://oeis.org/A043726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: search for numbers whose number of binary runs
   is congruent to 2 mod 4.  The function keeps an internal cache
   so that arbitrary order of calls is supported. */

#include <stdlib.h>  /* malloc, realloc */

static seq_t *cache = NULL;      /* cache of found terms */
static size_t cache_len = 0;     /* number of terms stored */
static size_t cache_cap = 0;     /* allocated capacity */
static idx_t next_start = 2;     /* next integer to test */

/* Grow cache capacity when needed */
static void grow_cache(void)
{
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    } else {
        cache_cap *= 2;
        cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
    }
}

/* Count the number of runs in the binary representation of v. */
static size_t run_count(idx_t v)
{
    size_t runs = 0;
    idx_t prev = 2;          /* value not 0 or 1, to force first run */
    while (v) {
        idx_t bit = v & 1;
        if (bit != prev) {
            ++runs;
            prev = bit;
        }
        v >>= 1;
    }
    return runs;
}

/* Compute the n-th term of the sequence (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n < 1, return 0 */

    while (cache_len < n) {
        /* test next_start for run-count congruent to 2 mod 4 */
        if ((run_count(next_start) & 3) == 2) {
            if (cache_len >= cache_cap)
                grow_cache();
            cache[cache_len++] = (seq_t)next_start;
        }
        ++next_start;
    }
    return cache[n - 1];
}
