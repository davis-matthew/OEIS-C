/*
 * A221224  Number of n X 3 arrays of occupancy after each element stays put
 *          or moves to some king-move neighbor.
 * https://oeis.org/A221224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 77*a(n-1) - 1710*a(n-2) + 13925*a(n-3) - 39617*a(n-4) +
 *            25616*a(n-5) - 5184*a(n-6) Initial terms: a(1)=8, a(2)=400,
 *            a(3)=18768, a(4)=867776, a(5)=39983152, a(6)=1840507104
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Lazy memoized array of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms currently stored (valid for 1..cache_len) */
    static size_t cache_cap = 0;   /* allocated capacity */

    if (cache_cap == 0) {
        cache_cap = 6;
        cache_len = 6;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) abort();
        cache[0] = (seq_t)8;
        cache[1] = (seq_t)400;
        cache[2] = (seq_t)18768;
        cache[3] = (seq_t)867776;
        cache[4] = (seq_t)39983152;
        cache[5] = (seq_t)1840507104;
    }

    if (n <= cache_len)
        return cache[n - 1];

    /* Extend cache up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* Grow storage if needed */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute next term using the six-term linear recurrence */
        seq_t pos = 0, neg = 0;
        /* positive contributions */
        pos += (seq_t)77     * cache[i - 2];
        pos += (seq_t)13925  * cache[i - 4];
        pos += (seq_t)25616  * cache[i - 6];
        /* negative contributions */
        neg += (seq_t)1710   * cache[i - 3];
        neg += (seq_t)39617  * cache[i - 5];
        neg += (seq_t)5184   * cache[i - 7];

        /* result is positive, so subtraction is safe in unsigned arithmetic */
        cache[cache_len++] = pos - neg;
    }

    return cache[n - 1];
}
