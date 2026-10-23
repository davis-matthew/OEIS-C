/*
 * A122852  Row sums of number triangle A122851.
 * https://oeis.org/A122852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (a(n-1) + n*a(n-2) + 1)/2 for n > 1 a(0) = 1, a(1) = 1
 *            (This recurrence generates the row sums of triangle A122851.)
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 22, u64 n <= 38, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to allow out‑of‑order queries */
    static seq_t *cache = NULL;   /* array of computed values */
    static idx_t  max_n = (idx_t)-1; /* largest index computed so far */
    static size_t cap   = 0;          /* allocated capacity of cache */

    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    /* allocate or grow cache if needed */
    size_t needed = (size_t)n + 1;
    if (!cache) {
        cache = (seq_t*)malloc(needed * sizeof(seq_t));
        cap   = needed;
    } else if (needed > cap) {
        cache = (seq_t*)realloc(cache, needed * sizeof(seq_t));
        cap   = needed;
    }

    /* initialise the first two values if cache was empty */
    if (max_n == (idx_t)-1) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        max_n = 1;
    }

    /* compute values up to n */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t val = ( cache[i-1]
                    + (seq_t)i * cache[i-2]
                    + (seq_t)1 ) / (seq_t)2;
        cache[i] = val;
    }

    max_n = n;
    return cache[n];
}
