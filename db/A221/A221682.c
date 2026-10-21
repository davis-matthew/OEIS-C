/*
 * A221682  Number of 0..7 arrays of length n with each element differing from
 *          at least one neighbor by 1 or less, starting with 0.
 * https://oeis.org/A221682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            5*a(n-1)+3*a(n-2)-16*a(n-3)+65*a(n-4)-14*a(n-5)+23*a(n-6)+2*a(n-7)+8*a(n-8)
 *            with initial values: a(1)=0, a(2)=2, a(3)=5, a(4)=44, a(5)=199,
 *            a(6)=1182, a(7)=6102, a(8)=33710
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 28, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;

    /* initialize first 8 terms if not already */
    if (cache_size == 0)
    {
        cache_cap = 8;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return (seq_t)0; /* allocation failed */

        cache[0] = (seq_t)0;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)44;
        cache[4] = (seq_t)199;
        cache[5] = (seq_t)1182;
        cache[6] = (seq_t)6102;
        cache[7] = (seq_t)33710;

        cache_size = 8;
    }

    size_t N = (size_t)n;
    if (N <= cache_size)
        return cache[N - 1];

    /* extend capacity if needed */
    if (N > cache_cap)
    {
        size_t new_cap = cache_cap;
        while (new_cap < N)
            new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0; /* allocation failed, return 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to N */
    for (size_t i = cache_size + 1; i <= N; ++i)
    {
        seq_t pos = cache[i - 2] * (seq_t)5
                  + cache[i - 3] * (seq_t)3
                  + cache[i - 5] * (seq_t)65
                  + cache[i - 7] * (seq_t)23
                  + cache[i - 8] * (seq_t)2
                  + cache[i - 9] * (seq_t)8;

        seq_t neg = cache[i - 4] * (seq_t)16
                  + cache[i - 6] * (seq_t)14;

        cache[i - 1] = pos - neg;
    }

    cache_size = N;
    return cache[N - 1];
}
