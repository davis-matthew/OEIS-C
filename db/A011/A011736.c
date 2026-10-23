/*
 * A011736  A binary m-sequence: expansion of reciprocal of x^23 + x^5 + 1
 *          (mod 2, shifted by 22 initial 0's).
 * https://oeis.org/A011736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0..21)=0, a(22)=1; for n>=23: a(n) = a(n-23) ^ a(n-5) (mod 2)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoised sequence values */
    static size_t cache_size = 0; /* number of elements stored */

    size_t nn = (size_t)n;

    if (nn >= cache_size)
    {
        size_t new_size = nn + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;  /* out‑of‑memory: return 0 (should never happen) */
        cache = tmp;

        /* compute new elements */
        for (size_t i = cache_size; i < new_size; ++i)
        {
            if (i < 22)
                cache[i] = (seq_t)0;
            else if (i == 22)
                cache[i] = (seq_t)1;
            else
                cache[i] = cache[i - 23] ^ cache[i - 5];
        }

        cache_size = new_size;
    }

    return cache[nn];
}
