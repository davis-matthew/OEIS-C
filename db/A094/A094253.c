/*
 * A094253  Let M be the 3 X 3 Matrix [ -4 4 8 / 1 0 0 / 0 1 0], a(n) =
 *          absolute value of the center term of M^n * [1 1 1].
 * https://oeis.org/A094253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=8, a(3)=20, a(4)=120,
 *            a(n)=4*a(n-1)+4*a(n-2)-8*a(n-3) for n>4. This recursion follows
 *            from the powers of the 3×3 matrix M = [[-4,4,8],[1,0,0],[0,1,0]]
 *            applied to [1 1 1]ᵗ and taking the absolute value of the middle
 *            entry. The implementation memoizes the sequence so that calls in
 *            arbi...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Values for n < 1 are undefined; return 0 for safety. */
    if (n < 1) return (seq_t)0;

    /* Static cache: cache[i] holds a(i+1).  cache_len is the number of
       terms currently stored (>= 4).  cache_size is the allocated size. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_size = 0;

    /* Initialise cache on the first call. */
    if (cache_len == 0)
    {
        /* Reserve space for the first 4 terms. */
        cache_size = 8;                 /* start with some slack */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;    /* allocation failure => 0 */

        cache[0] = (seq_t)1;   /* a(1) */
        cache[1] = (seq_t)8;   /* a(2) */
        cache[2] = (seq_t)20;  /* a(3) */
        cache[3] = (seq_t)120; /* a(4) */
        cache_len = 4;
    }

    /* Extend cache if needed. */
    while (n > cache_len)
    {
        if (cache_len >= cache_size)
        {
            /* Double the buffer size. */
            idx_t new_size = cache_size * 2;
            seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
            if (!new_cache) return cache[cache_len - 1]; /* keep last value */
            cache = new_cache;
            cache_size = new_size;
        }

        /* Compute term a(cache_len + 1). */
        seq_t t1 = cache[cache_len - 1];   /* a(n-1)  */
        seq_t t2 = cache[cache_len - 2];   /* a(n-2)  */
        seq_t t3 = cache[cache_len - 3];   /* a(n-3)  */
        seq_t next = 4*t1 + 4*t2 - 8*t3;
        cache[cache_len] = next;
        cache_len++;
    }

    /* cache[n-1] now holds a(n). */
    return cache[n-1];
}
