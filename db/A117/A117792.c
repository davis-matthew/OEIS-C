/*
 * A117792  First entry of the vector (M^n)w, where M is the 6x6 matrix [[0,
 *          1, 0, 0, 0, 0, ], [0, 0, 1, 0, 0, 0], [0, 0, 0, 1, 0, 0], [0, 0,
 *          0, 0, 1, 0], [0, 0, 0, 0, 0, 1], [1, 0, -1, 0, 1, 1]] and w is the
 *          column vector [0, 1, 1, 2, 3, 5].
 * https://oeis.org/A117792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n‑1) + a(n‑2) – a(n‑4) + a(n‑6) with a(0)=0, a(1)=1,
 *            a(2)=1, a(3)=2, a(4)=3, a(5)=5
 * Range:     u8 n <= 14, u16 n <= 28, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n >= cache_len)
    {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) exit(1);          /* allocation failure */
        cache = tmp;

        for (idx_t i = cache_len; i < new_len; ++i)
        {
            if (i == 0)          cache[i] = (seq_t)0;
            else if (i == 1)     cache[i] = (seq_t)1;
            else if (i == 2)     cache[i] = (seq_t)1;
            else if (i == 3)     cache[i] = (seq_t)2;
            else if (i == 4)     cache[i] = (seq_t)3;
            else if (i == 5)     cache[i] = (seq_t)5;
            else
            {
                seq_t val = cache[i-1] + cache[i-2];
                val -= cache[i-4];
                val += cache[i-6];
                cache[i] = val;
            }
        }
        cache_len = new_len;
    }

    return cache[n];
}
