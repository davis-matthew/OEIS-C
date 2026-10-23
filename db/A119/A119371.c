/*
 * A119371  G.f. satisfies: A(x) = (1+x) - x*(4+x)*A(x) + x*(3+2*x)*A(x)^2.
 * https://oeis.org/A119371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 34, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A119371 --
 *  Recurrence (3*(n+1)*a(n) = 2*(5*n-4)*a(n-1) + 2*(7*n-8)*a(n-2) +
 *                4*(n-2)*a(n-3) - 3*(n-5)*a(n-4) - 2*(n-5)*a(n-5))
 *  a(0)=1, a(1)=0, a(2)=1, a(3)=2, a(4)=7
 *  Memoised for arbitrary call order.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <type_traits>
#endif

seq_t a(idx_t n)
{
    /* dynamic memoisation cache */
    static seq_t *cache = NULL;
    static size_t cap = 0;      /* allocated capacity */
    static size_t len = 0;      /* number of elements stored */

    /* grow the cache if necessary */
    if (cap < (size_t)(n + 1)) {
        size_t newcap = cap ? cap * 2 : 8;
        if (newcap < (size_t)(n + 1)) newcap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();     /* out of memory */
        cache = tmp;
        cap   = newcap;
    }

    /* initialise the five known terms only once */
    if (len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)2;
        cache[4] = (seq_t)7;
        len = 5;
    }

    /* if the desired term is already cached, return it */
    if (n < (idx_t)len) return cache[n];

    /* compute all missing terms up to n */
    for (idx_t i = (idx_t)len; i <= n; ++i) {
        seq_t val;
#if defined(__cplusplus)
        if constexpr (std::is_integral_v<seq_t>) {
            __int128 t = 0;
            t += (__int128)2 * (5*i-4) * (long long)cache[i-1];
            t += (__int128)2 * (7*i-8) * (long long)cache[i-2];
            t += (__int128)4 * (i-2)   * (long long)cache[i-3];
            t -= (__int128)3 * (i-5)   * (long long)cache[i-4];
            t -= (__int128)2 * (i-5)   * (long long)cache[i-5];
            val = (seq_t)(t / (3*(i+1)));
        } else {
            val = seq_t(2) * seq_t(5*i-4) * cache[i-1];
            val += seq_t(2) * seq_t(7*i-8) * cache[i-2];
            val += seq_t(4) * seq_t(i-2) * cache[i-3];
            val -= seq_t(3) * seq_t(i-5) * cache[i-4];
            val -= seq_t(2) * seq_t(i-5) * cache[i-5];
            val  = val / seq_t(3*(i+1));
        }
#else   /* pure C: seq_t is integral */
        __int128 t = 0;
        t += (__int128)2 * (5*i-4) * (long long)cache[i-1];
        t += (__int128)2 * (7*i-8) * (long long)cache[i-2];
        t += (__int128)4 * (i-2)   * (long long)cache[i-3];
        t -= (__int128)3 * (i-5)   * (long long)cache[i-4];
        t -= (__int128)2 * (i-5)   * (long long)cache[i-5];
        val = (seq_t)(t / (3*(i+1)));
#endif
        cache[i] = val;
    }

    len = (size_t)(n + 1);
    return cache[n];
}
