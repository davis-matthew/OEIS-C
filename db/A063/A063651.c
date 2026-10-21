/*
 * A063651  Number of ways to tile a 7 X n rectangle with 1 X 1 and 2 X 2
 *          tiles.
 * https://oeis.org/A063651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1,a(1)=1,a(2)=21,a(3)=85,a(4)=747,a(5)=4375,a(6)=31387,a(7)=202841;
 *            a(n)=3a(n-1)+30a(n-2)-17a(n-3)-138a(n-4)+85a(n-5)+116a(n-6)-42a(n-7)-32a(n-8)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n >= cache_size) {
        idx_t required = n + 1;
        idx_t new_size = cache_size ? cache_size : 8;
        while (new_size <= n) new_size <<= 1;

        idx_t old_size = cache_size;
        seq_t *new_cache;

#ifdef __cplusplus
        new_cache = new seq_t[new_size];
        if (cache) {
            for (idx_t i = 0; i < old_size; ++i) new_cache[i] = cache[i];
            delete[] cache;
        }
#else
        new_cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
#endif
        if (!new_cache) return 0;  /* allocation failure fallback */

        cache = new_cache;
        cache_size = new_size;

        /* Initialize base terms if not yet present */
        if (old_size < 8) {
            if (new_size >= 8) {
                cache[0] = 1;
                cache[1] = 1;
                cache[2] = 21;
                cache[3] = 85;
                cache[4] = 747;
                cache[5] = 4375;
                cache[6] = 31387;
                cache[7] = 202841;
            }
        }

        /* Compute new terms */
        for (idx_t i = (old_size > 8 ? old_size : 8); i < new_size; ++i) {
            seq_t val = 0;
            val += 3 * cache[i-1];
            val += 30 * cache[i-2];
            val -= 17 * cache[i-3];
            val -= 138 * cache[i-4];
            val += 85 * cache[i-5];
            val += 116 * cache[i-6];
            val -= 42 * cache[i-7];
            val -= 32 * cache[i-8];
            cache[i] = val;
        }
    }

    return cache[n];
}
