/*
 * A085939  Horadam sequence (0,1,6,4).
 * https://oeis.org/A085939
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A085939 – Horadam sequence (0,1,6,4)
    Formula:  a(0) = 0,  a(1) = 1,
              a(n) = 4*a(n-1) + 6*a(n-2)      for n > 1
*/
#include <stdlib.h>
#include <stddef.h>

/* --- Utility: grow a seq_t array to at least new_cap elements --- */
static void grow_seq_array(seq_t **arr, size_t *cap, size_t new_cap) {
    if (*cap >= new_cap) return;
    size_t new_cap2 = (*cap == 0) ? 8 : (*cap << 1);
    while (new_cap2 < new_cap) new_cap2 <<= 1;
    seq_t *tmp = (seq_t *)realloc(*arr, new_cap2 * sizeof(seq_t));
    if (!tmp) abort();                     /* out of memory – abort */
    *arr = tmp;
    *cap = new_cap2;
}

/* --- Cache for memoisation ------------------------------------- */
static seq_t *cache = NULL;      /* array for computed values */
static size_t cache_cap = 0;     /* allocated capacity               */
static size_t cache_len = 0;     /* number of elements actually set   */

seq_t a(idx_t n) {
    /* Base cases handled directly for speed and clarity */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Ensure the cache can hold up to index n */
    grow_seq_array(&cache, &cache_cap, (size_t)n + 1);

    /* Initialise base values once */
    if (cache_len == 0) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_len = 2;                     /* cache[0] and cache[1] set */
    }

    /* Compute missing values up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        /* Because cache_len >= 2, i-1 and i-2 are valid indices */
        seq_t prev1 = cache[i - 1];
        seq_t prev2 = cache[i - 2];
        cache[i] = (seq_t)((seq_t)4 * prev1 + (seq_t)6 * prev2);
    }
    cache_len = (size_t)n + 1;                 /* all indices 0..n now set */

    return cache[(size_t)n];
}
