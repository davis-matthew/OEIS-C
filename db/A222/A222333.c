/*
 * A222333  Number of binary arrays indicating the locations of trailing edge
 *          maxima of a random length-n 0..7 array extended with zeros and
 *          convolved with 1,1.
 * https://oeis.org/A222333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            a(n-1)+a(n-3)+a(n-5)+a(n-7)+a(n-9)+a(n-11)+a(n-13)+a(n-15);
 *            initial values: a(1)=2, a(2)=3, a(3)=4, a(4)=7, a(5)=11,
 *            a(6)=18, a(7)=29, a(8)=47, a(9)=76, a(10)=123, a(11)=199,
 *            a(12)=322, a(13)=521, a(14)=843, a(15)=1364, a(16)=2206
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    if (n == 0)      /* offset is 1; safe guard for illegal use */
        return (seq_t)0;

    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static idx_t max_computed = 0;
    static int initialized = 0;

    /* Initialise cache on first call */
    if (!initialized)
    {
        const idx_t init_count = 16;   /* indices 0..15 inclusive */
        cache_size = init_count;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache)                     /* allocation failure – best effort */
            return (seq_t)0;

        /* zero‑initialise the array */
        for (size_t i = 0; i < cache_size; ++i)
            cache[i] = (seq_t)0;

        /* pre‑filled terms as strings (safe for any seq_t implementation) */
        static const char *init_strs[] = {
            "0",   /* dummy for index 0 */
            "2", "3", "4", "7", "11", "18", "29", "47",
            "76", "123", "199", "322", "521", "843", "1364"
        };
        for (idx_t i = 1; i < init_count; ++i)
            cache[i] = seq_from_str(init_strs[i]);

        max_computed = 15;   /* last pre‑computed index */
        initialized   = 1;
    }

    /* Grow the cache if necessary */
    if ((size_t)(n + 1) > cache_size)
    {
        size_t new_size = (size_t)(n + 1);
        if (new_size < cache_size * 2)
            new_size = cache_size * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)       /* allocation failure – best effort */
            return (seq_t)0;
        /* initialise newly allocated entries to zero */
        for (size_t i = cache_size; i < new_size; ++i)
            tmp[i] = (seq_t)0;
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute missing values using the recurrence */
    if (n > max_computed)
    {
        for (idx_t i = max_computed + 1; i <= n; ++i)
        {
            seq_t sum = (seq_t)0;
            for (int k = 1; k <= 15; k += 2)
                sum += cache[i - k];
            cache[i] = sum;
        }
        max_computed = n;
    }

    return cache[n];
}
