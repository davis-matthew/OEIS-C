/*
 * A197369  Number of n X 4 0..4 arrays with each element x equal to the
 *          number of its horizontal and vertical neighbors equal to 2,4,1,0,1
 *          for x=0,1,2,3,4.
 * https://oeis.org/A197369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 10*a(n-2) + 18*a(n-3) * - 7*a(n-4) - 33*a(n-5)
 *            - 58*a(n-6) * + 13*a(n-7) + 22*a(n-8) - 31*a(n-9) * + 18*a(n-10)
 *            - 3*a(n-11) (n > 11) * Initial values: a(1)=1, a(2)=7, a(3)=45,
 *            a(4)=161, a(5)=899, a(6)=4023, * a(7)=19387, a(8)=92169,
 *            a(9)=436539, a(10)=2077281, * a(11)=9861263
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for offset 1 */

    /* Static cache for previously computed terms */
    static seq_t *cache = NULL;
    static size_t cached_size = 0;        /* number of terms stored (>=0) */
    static int initialized = 0;

    size_t N = (size_t)n;                 /* required length of cache */

    /* Initialise cache with first 11 terms */
    if (!initialized) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 11);
        if (!cache) return 0;            /* malloc failure */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)7;
        cache[2] = (seq_t)45;
        cache[3] = (seq_t)161;
        cache[4] = (seq_t)899;
        cache[5] = (seq_t)4023;
        cache[6] = (seq_t)19387;
        cache[7] = (seq_t)92169;
        cache[8] = (seq_t)436539;
        cache[9] = (seq_t)2077281;
        cache[10] = (seq_t)9861263;
        cached_size = 11;
        initialized = 1;
    }

    /* Grow the cache if necessary */
    if (N > cached_size) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * N);
        if (!tmp) return 0;              /* realloc failure */
        cache = tmp;
        /* Compute new terms using the recurrence */
        for (size_t idx = cached_size; idx < N; ++idx) {
            seq_t pos = 0, neg = 0;
            /* Positive coefficients */
            pos += (seq_t)2 *  cache[idx-1];
            pos += (seq_t)10 * cache[idx-2];
            pos += (seq_t)18 * cache[idx-3];
            pos += (seq_t)13 * cache[idx-7];
            pos += (seq_t)22 * cache[idx-8];
            pos += (seq_t)18 * cache[idx-10];
            /* Negative coefficients */
            neg += (seq_t)7  * cache[idx-4];
            neg += (seq_t)33 * cache[idx-5];
            neg += (seq_t)58 * cache[idx-6];
            neg += (seq_t)31 * cache[idx-9];
            neg += (seq_t)3  * cache[idx-11];
            cache[idx] = pos - neg;
        }
        cached_size = N;
    }

    return cache[N-1];
}
