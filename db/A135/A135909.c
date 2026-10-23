/*
 * A135909  Clique number of commuting graph of alternating group A_n.
 * https://oeis.org/A135909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-3) - 3*a(n-4) for n ≥ 10; Initial values:
 *            a(0)=0, a(1)=0, a(2)=0, a(3)=2, a(4)=3, a(5)=4, a(6)=8, a(7)=11,
 *            a(8)=15, a(9)=26. (The recurrence is conjectured for n>6; it
 *            holds for all n≥10 in the known data.)
 * Range:     u8 n <= 15, u16 n <= 30, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoisation array */
static idx_t max_cached = (idx_t)-1; /* last index stored */

seq_t a(idx_t n)
{
    /* Initialise cache with first 10 known terms. */
    if (cache == NULL) {
        cache = (seq_t *)malloc(10 * sizeof(seq_t));
        if (cache == NULL) return 0;  /* abort on memory failure */
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 0;
        cache[3] = 2;
        cache[4] = 3;
        cache[5] = 4;
        cache[6] = 8;
        cache[7] = 11;
        cache[8] = 15;
        cache[9] = 26;
        max_cached = 9;
    }

    /* If the requested term is already computed, return it. */
    if (n <= max_cached) {
        return cache[n];
    }

    /* Extend cache to hold up to n. */
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (new_cache == NULL) return 0;  /* abort on memory failure */
    cache = new_cache;

    /* Compute terms sequentially from max_cached+1 up to n. */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        /* Recurrence: a(i) = a(i-1) + 3*a(i-3) - 3*a(i-4)   (i ≥ 10) */
        cache[i] = cache[i-1] + (3 * cache[i-3]) - (3 * cache[i-4]);
    }

    max_cached = n;
    return cache[n];
}
