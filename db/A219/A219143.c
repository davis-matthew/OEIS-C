/*
 * A219143  Unchanging value maps: number of 2 X n binary arrays indicating
 *          the locations of corresponding elements unequal to no horizontal,
 *          diagonal or antidiagonal neighbor in a random 0..2 2 X n array.
 * https://oeis.org/A219143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - a(n-3) + 2*a(n-4) (n>5) Initial
 *            values: a(1)=1, a(2)=6, a(3)=18, a(4)=39, a(5)=96
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 50, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for n<1, return 0 */

    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise cache with first 5 terms */
    if (cache == NULL) {
        cache_size = 5;
        cache = (seq_t *)calloc(cache_size + 1, sizeof(seq_t)); /* index 0 unused */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)18;
        cache[4] = (seq_t)39;
        cache[5] = (seq_t)96;
    }

    /* extend cache to n if necessary */
    if (n > cache_size) {
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        if (!new_cache) return (seq_t)0; /* allocation failure, return 0 */
        cache = new_cache;
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            cache[i] = (2 * cache[i - 1]) + cache[i - 2] - cache[i - 3] + (2 * cache[i - 4]);
        }
        cache_size = n;
    }

    return cache[n];
}
