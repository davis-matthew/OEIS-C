/*
 * A017833  Expansion of 1/(1-x^4-x^5-x^6-x^7-x^8-x^9-x^10-x^11).
 * https://oeis.org/A017833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 41, u32 n <= 77, u64 n <= 149,
 *            u128 n <= 292, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-4)+a(n-5)+a(n-6)+a(n-7)+a(n-8)+a(n-9)+a(n-10)+a(n-11)
 * for n>10 with
 *   a(0)=1, a(1)=0, a(2)=0, a(3)=0,
 *   a(4)=1, a(5)=1, a(6)=1, a(7)=1,
 *   a(8)=2, a(9)=3, a(10)=4, a(11)=5
 * This function memoises the sequence across calls.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* cached values a(0)...a(k-1) */
    static size_t cache_size = 0;    /* number of entries currently stored */

    /* initialise cache if not yet done, with the first 12 terms */
    if (cache_size == 0) {
        cache_size = 12;                     /* indices 0..11 inclusive */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) { exit(1); }      /* abort on allocation failure */

        cache[0] = 1;
        cache[1] = cache[2] = cache[3] = 0;
        cache[4] = cache[5] = cache[6] = cache[7] = 1;
        cache[8] = 2;
        cache[9] = 3;
        cache[10] = 4;
        cache[11] = 5;
    }

    /* extend cache if the requested index is beyond the current range */
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;                /* need indices 0..n */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) { exit(1); }
        cache = tmp;

        /* compute new values using the recurrence for i >= 12 */
        for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
            seq_t sum = 0;
            sum += cache[i-4];
            sum += cache[i-5];
            sum += cache[i-6];
            sum += cache[i-7];
            sum += cache[i-8];
            sum += cache[i-9];
            sum += cache[i-10];
            sum += cache[i-11];
            cache[i] = sum;
        }
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
