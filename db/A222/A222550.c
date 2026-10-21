/*
 * A222550  Number of (n+3) X 1 arrays of occupancy after each element moves
 *          up to +-3 places but not 0.
 * https://oeis.org/A222550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 15, u64 n <= 33, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 7*a(n-1) -15*a(n-2) +10*a(n-3) +4*a(n-4) -36*a(n-5)
   +73*a(n-6) -29*a(n-7) -21*a(n-8) -a(n-9)   for n>9
   with initial terms
   a(1)=31, a(2)=102, a(3)=359, a(4)=1279, a(5)=4537,
   a(6)=15929, a(7)=56041, a(8)=197313, a(9)=694561  */

#include <stdlib.h>

static const seq_t init_vals[10] = {
    (seq_t)0,          /* unused 0 */
    (seq_t)31, (seq_t)102, (seq_t)359, (seq_t)1279,
    (seq_t)4537,  (seq_t)15929, (seq_t)56041,
    (seq_t)197313, (seq_t)694561
};

static seq_t *cache = NULL;     /* 1‑based cache of terms */
static idx_t   cache_cap = 0;   /* size of allocated array */
static idx_t   cache_last = 0;  /* largest index that has been computed */

/* Ensure cache can hold terms up to index n (1‑based). */
static void ensure_cache(idx_t n)
{
    if (cache_cap <= n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) {
            /* allocation failure – exit safely */
            exit(EXIT_FAILURE);
        }
        cache = tmp;
        cache_cap = (idx_t)new_cap;
    }
}

/* Compute a(n) for any n ≥ 1. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined below offset */

    if (n <= 9) return init_vals[n];    /* return direct if within initial terms */

    ensure_cache(n);

    /* Initialise the first nine entries if not already done. */
    if (cache_last < 9) {
        /* If some entries already exist, preserve them; otherwise set from init_vals. */
        for (idx_t i = cache_last + 1; i <= 9; ++i) {
            if (cache[i] == (seq_t)0)          /* zero indicates uncomputed */
                cache[i] = init_vals[i];
        }
        cache_last = 9;
    }

    /* Compute all missing terms up to n. */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        cache[i] =
            7   * cache[i-1]
          - 15  * cache[i-2]
          + 10  * cache[i-3]
          + 4   * cache[i-4]
          - 36  * cache[i-5]
          + 73  * cache[i-6]
          - 29  * cache[i-7]
          - 21  * cache[i-8]
          - cache[i-9];
    }
    cache_last = n;
    return cache[n];
}
