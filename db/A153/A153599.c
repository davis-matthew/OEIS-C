/*
 * A153599  a(n) = ((8 + sqrt(3))^n - (8 - sqrt(3))^n)/(2*sqrt(3)).
 * https://oeis.org/A153599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((8+√3)^n - (8-√3)^n) / (2√3) * Recurrence (OEIS):
 *            a(1)=1, a(2)=16, * a(n) = 16*a(n-1) - 61*a(n-2) for n>2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* highest index stored (0 means none) */

    /* Ensure base values are ready */
    if (cache_len < 2) {
        size_t new_len = 2;          /* we need indices 1 and 2 */
        cache = (seq_t *)realloc(cache, (new_len + 1) * sizeof(seq_t));
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)16;
        cache_len = new_len;
    }

    /* Expand cache if needed */
    while (cache_len < n) {
        idx_t i = cache_len;                 /* current highest index */
        /* a(i+1) = 16*a(i) - 61*a(i-1) */
        seq_t term = (seq_t)16 * cache[i] - (seq_t)61 * cache[i - 1];
        cache_len++;
        cache = (seq_t *)realloc(cache, (cache_len + 1) * sizeof(seq_t));
        cache[cache_len] = term;
    }

    return cache[n];
}
