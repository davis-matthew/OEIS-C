/*
 * A050401  Number of independent sets of nodes in P_4 X C_n (n > 2).
 * https://oeis.org/A050401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 20*a(n-2) + 27*a(n-3) - 14*a(n-4) - 25*a(n-5) +
 *            4*a(n-6) + 5*a(n-7) - a(n-8) Base values: a(0)=8, a(1)=1,
 *            a(2)=41, a(3)=142, a(4)=933, a(5)=4741, a(6)=26660, a(7)=143697
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static data: cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t len   = 0; /* number of terms stored */
    static size_t cap   = 0; /* allocated capacity */

    /* initialise cache with first 8 terms */
    if (cache == NULL) {
        cap = 8;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;               /* allocation failed */
        cache[0] = (seq_t)8;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)41;
        cache[3] = (seq_t)142;
        cache[4] = (seq_t)933;
        cache[5] = (seq_t)4741;
        cache[6] = (seq_t)26660;
        cache[7] = (seq_t)143697;
        len   = 8;
    }

    /* grow the cache if it must hold more terms */
    if (len <= (size_t)n) {
        /* ensure the array is large enough for the new term */
        if (len == cap) {
            size_t new_cap = cap * 2;
            if (new_cap == 0) new_cap = 1;
            if (new_cap < len + 1) new_cap = len + 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;           /* allocation failed */
            cache = tmp;
            cap = new_cap;
        }

        /* compute missing terms up to n (inclusive) */
        while (len <= (size_t)n) {
            /* recurrence for index = len */
            seq_t val = cache[len - 1];
            val += (seq_t)20 * cache[len - 2];
            val += (seq_t)27 * cache[len - 3];
            val -= (seq_t)14 * cache[len - 4];
            val -= (seq_t)25 * cache[len - 5];
            val += (seq_t) 4 * cache[len - 6];
            val += (seq_t) 5 * cache[len - 7];
            val -=      cache[len - 8];

            /* ensure we have space for this new term */
            if (len == cap) {
                size_t new_cap = cap * 2;
                if (new_cap == 0) new_cap = 1;
                if (new_cap < len + 1) new_cap = len + 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;
                cache = tmp;
                cap = new_cap;
            }

            cache[len] = val;
            len++;
        }
    }

    return cache[(size_t)n];
}
