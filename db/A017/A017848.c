/*
 * A017848  Expansion of 1/(1-x^6-x^7-x^8).
 * https://oeis.org/A017848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=a(2)=a(3)=a(4)=a(5)=0, * a(6)=a(7)=a(8)=1;
 *            a(n)=a(n-6)+a(n-7)+a(n-8) for n>8
 * Range:     u8 n <= 46, u16 n <= 82, u32 n <= 152, u64 n <= 292,
 *            u128 n <= 573, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* dynamic cache of computed terms */
    static idx_t cache_len = 0;      /* number of elements currently stored in cache */

    if (n + 1 > cache_len) {
        idx_t new_len = n + 1;
        /* realloc casted for C and C++ compatibility */
        seq_t *tmp = (seq_t*)realloc((void*)cache, (size_t)new_len * sizeof(seq_t));
        if (!tmp)          /* allocation failure: return 0 (unlikely) */
            return (seq_t)0;

        /* initialise new entries */
        for (idx_t i = cache_len; i < new_len; ++i) {
            if (i == 0)
                tmp[i] = (seq_t)1;
            else if (i < 6)         /* 1..5 */
                tmp[i] = (seq_t)0;
            else if (i == 6 || i == 7 || i == 8)
                tmp[i] = (seq_t)1;
            else                    /* recurrence */
                tmp[i] = tmp[i-6] + tmp[i-7] + tmp[i-8];
        }

        cache      = tmp;
        cache_len  = new_len;
    }

    return cache[n];
}
