/*
 * A016283  a(n) = 6^n/8 - 4^(n-1) + 2^(n-3).
 * https://oeis.org/A016283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) - 44*a(n-2) + 48*a(n-3), with a(0)=0, a(1)=0,
 *            a(2)=1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_mul.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of elements currently stored */

    if (n >= cache_len) {
        idx_t old_len = cache_len;
        idx_t new_len = n + 1;
        seq_t *new_cache = seq_alloc_new_array(new_len);
        for (idx_t i = 0; i < old_len; ++i)
            new_cache[i] = cache[i];
        if (cache)
            seq_free(cache);
        cache = new_cache;

        for (idx_t i = old_len; i <= n; ++i) {
            if (i == 0)
                cache[i] = (seq_t)0;
            else if (i == 1)
                cache[i] = (seq_t)0;
            else if (i == 2)
                cache[i] = (seq_t)1;
            else {
                seq_t t1 = seq_mul(cache[i - 1], (seq_t)12);
                seq_t t2 = seq_mul(cache[i - 2], (seq_t)44);
                seq_t t3 = seq_mul(cache[i - 3], (seq_t)48);
                cache[i] = t1 - t2 + t3;
            }
        }
        cache_len = new_len;
    }
    return cache[n];
}
