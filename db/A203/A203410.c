/*
 * A203410  Indices of decagonal numbers that are also heptagonal.
 * https://oeis.org/A203410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A203410 – Indices of decagonal numbers that are also heptagonal
   Recurrence: a(n) = 1442*a(n-2) - a(n-4) - 540   for n >= 5
   Initial values:
      a(1) = 1
      a(2) = 12
      a(3) = 850
      a(4) = 16761
   The function computes any term a(n) (n >= 1) on demand, memoising
   computed values in a static array.
   It is safe for any call order and runs in O(n) time for the largest
   requested n.  Intermediate arithmetic stays within seq_t.  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: index 0 unused; indices 1..k contain a(1)..a(k) */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of allocated slots (cache_len >= 1) */
    if (n == (idx_t)0) return 0;   /* undefined for n==0; return 0 */

    /* Ensure cache covers indices up to at least 4 */
    if (cache_len == 0) {
        cache_len = 5;  /* indices 0..4 */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        /* initialise all to zero */
        for (size_t i = 0; i < cache_len; ++i) cache[i] = 0;
        /* set initial values */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)12;
        cache[3] = (seq_t)850;
        cache[4] = (seq_t)16761;
    }

    /* Expand cache if needed */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;  /* allocation failed – return 0 as sentinel */
        cache = tmp;
        /* initialise new slots to zero */
        for (size_t i = cache_len; i < new_len; ++i) cache[i] = 0;
        cache_len = new_len;
    }

    /* Compute missing terms using recurrence */
    for (size_t i = 1; i <= (size_t)n; ++i) {
        if (i <= 4) continue;          /* already initialised */
        if (cache[i] != 0) continue;    /* already computed (e.g., due to previous call) */
        cache[i] = (seq_t)(1442 * cache[i - 2] - cache[i - 4] - 540);
    }

    return cache[(size_t)n];
}
