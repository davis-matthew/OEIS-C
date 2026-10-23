/*
 * A008677  Expansion of 1/((1-x^3)*(1-x^5)*(1-x^7)).
 * https://oeis.org/A008677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-3)+a(n-5)+a(n-7)-a(n-8)-a(n-10)-a(n-12)+a(n-15) with
 *            a(0)=1, a(1)=a(2)=0
 * Range:     u8 n <= 224, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq, seq_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq.h"
#include "../utils/seq_free.h"

seq_t a(idx_t n)
{
    /* static cache ---------------------------------------------------*/
    static seq_t *cache = NULL;   /* pointer to stored values */
    static idx_t  cache_len = 0;  /* number of values currently stored */

    /* ensure we have values up to index n -----------------------------*/
    if (n >= cache_len) {
        size_t new_len = (size_t)n + 1;          /* new cache size      */
        seq_t *new_cache = alloc_seq(new_len);   /* allocate new array   */

        /* copy old values into the new array */
        for (idx_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];

        /* fill missing entries from cache_len up to n */
        for (idx_t i = cache_len; i < (idx_t)new_len; ++i) {
            seq_t val;
            if (i == 0) {
                val = (seq_t)1;          /* a(0)=1 */
            } else if (i < 3) {
                val = (seq_t)0;          /* a(1)=a(2)=0 */
            } else {
                val = 0;
                if (i >= 3) val += new_cache[i - 3];
                if (i >= 5) val += new_cache[i - 5];
                if (i >= 7) val += new_cache[i - 7];
                if (i >= 8) val -= new_cache[i - 8];
                if (i >= 10) val -= new_cache[i - 10];
                if (i >= 12) val -= new_cache[i - 12];
                if (i >= 15) val += new_cache[i - 15];
            }
            new_cache[i] = val;
        }

        /* free old cache and update the static pointers */
        if (cache)
            seq_free(cache);          /* frees array and destroys elements */
        cache = new_cache;
        cache_len = (idx_t)new_len;
    }

    /* return the stored value ----------------------------------------*/
    return cache[n];
}
