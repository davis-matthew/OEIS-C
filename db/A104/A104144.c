/*
 * A104144  a(n) = Sum_{k=1..9} a(n-k); a(8) = 1, a(n) = 0 for n < 8.
 * https://oeis.org/A104144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..9} a(n-k) for n > 8, a(8) = 1, a(n) = 0 for n <
 *            8
 * Range:     u8 n <= 16, u16 n <= 25, u32 n <= 41, u64 n <= 73, u128 n <= 137,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* dynamic array holding computed values */
static idx_t  cache_size = 0; /* number of allocated elements (capacity) */

/* expand cache to hold at least indices up to n and compute any missing values */
static void ensure_cache(idx_t n)
{
    /* initial allocation and seed values (if first call) */
    if (cache_size == 0) {
        idx_t cap = 16;                    /* start with a moderate capacity */
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
        if (!cache) abort();                /* out of memory */

        /* values for indices 0..7 are 0 (already zeroed by calloc) */
        cache[8] = (seq_t)1;                /* a(8) = 1 */
        cache_size = cap;                   /* capacity now = 16 */
    }

    /* enlarge if capacity is insufficient for desired index n */
    if (n + 1 > cache_size) {
        idx_t new_cap = cache_size;
        while (n + 1 > new_cap) new_cap <<= 1;   /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        /* new memory is uninitialised; will be filled by the compute loop */
        cache = tmp;
        cache_size = new_cap;
    }

    /* compute any values that have not yet been calculated (start at 9) */
    /* we keep track of the highest index that has a valid value in the
       array by examining the array from the back until we hit a non zero
       for indices <= 8 or zero for indices >8 but unknown. Since all
       a(n) for n>8 are positive, any zero in that range means it hasn't
       been computed yet. */
    static idx_t computed_upto = 0;          /* highest index computed so far */

    /* initialise computed_upto on first run */
    if (computed_upto == 0) {
        /* a(0)..a(7) = 0, a(8)=1, so computed_upto = 8 */
        computed_upto = 8;
    }

    /* If requested index is already computed, nothing more to do */
    if (n <= computed_upto)
        return;

    /* compute from computed_upto+1 up to n */
    for (idx_t i = computed_upto + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (int j = 1; j <= 9; ++j)
            sum += cache[i - j];
        cache[i] = sum;
    }
    computed_upto = n;
}

seq_t a(idx_t n)
{
    if (n < 8)
        return (seq_t)0;         /* a(0)..a(7) are zero */
    ensure_cache(n);
    return cache[n];
}
