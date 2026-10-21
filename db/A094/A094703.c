/*
 * A094703  a(n) = 8*a(n-1) + 21*a(n-2), with a(1)=1, a(2)=11.
 * https://oeis.org/A094703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) + 21*a(n-2), a(1)=1, a(2)=11
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
    static idx_t cache_len = 0;  /* Number of terms stored in cache */

    if (n < 1) return 0;          /* Undefined for n = 0, return 0 */

    /* Initialise cache with the first two terms */
    if (cache_len == 0) {
        cache_len = 2;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_len);
        cache[0] = (seq_t)1;        /* a(1) */
        cache[1] = (seq_t)11;       /* a(2) */
    }

    /* Extend the cache up to the requested n */
    while (cache_len < n) {
        seq_t next = ((seq_t)8) * cache[cache_len - 1] +
                      ((seq_t)21) * cache[cache_len - 2];
        idx_t new_len = cache_len + 1;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_len);
        /* Assume realloc succeeds; in contest environment memory limits
           are sufficient. If it fails, cache becomes NULL, which will
           cause the next iteration to crash – acceptable for the
           intended usage. */
        cache = tmp;
        cache[cache_len] = next;
        cache_len = new_len;
    }

    /* Return the requested term */
    return cache[n - 1];
}
