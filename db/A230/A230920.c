/*
 * A230920  Number of n X 2 0..2 arrays x(i,j) with each element horizontally,
 *          vertically, diagonally or antidiagonally next to at least one
 *          element with value (x(i,j)+1) mod 3 and at least one element with
 *          value (x(i,j)-1) mod 3, and upper left element zero.
 * https://oeis.org/A230920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0, a(2)=12, a(3)=54, a(4)=432, a(5)=3108 Recurrence: a(n) =
 *            5*a(n-1) + 14*a(n-2) + 16*a(n-3) - 2*a(n-4) + 4*a(n-5) (n ≥ 6)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    if (n == 0) return (seq_t)0;

    /* Initialize cache with first six entries (index 0 unused). */
    if (cache == NULL) {
        cache_len = 6; /* allocate indices 0..5 */
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* Allocation failure: return 0 as fallback */
        cache[0] = (seq_t)0;          /* unused */
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)12;
        cache[3] = (seq_t)54;
        cache[4] = (seq_t)432;
        cache[5] = (seq_t)3108;
    }

    size_t idx = (size_t)n;
    if (idx >= cache_len) {
        size_t new_len = idx + 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) return (seq_t)0; /* Allocation failure: return 0 as fallback */
        cache = new_cache;
        for (size_t i = cache_len; i <= idx; ++i) {
            if (i <= 5) continue; /* indices 1..5 already set */
            /* Compute via recurrence, casting constants to seq_t */
            seq_t term = ((seq_t)5)*cache[i-1]
                       + ((seq_t)14)*cache[i-2]
                       + ((seq_t)16)*cache[i-3]
                       + ((seq_t)4)*cache[i-5]
                       - ((seq_t)2)*cache[i-4];
            cache[i] = term;
        }
        cache_len = new_len;
    }

    return cache[idx];
}
