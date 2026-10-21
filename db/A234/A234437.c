/*
 * A234437  Number of (n+1) X (2+1) 0..2 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 1
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + 24*a(n-2) - 130*a(n-3) + 36*a(n-4) + 80*a(n-5)
 *            - 32*a(n-6) for n ≥ 7, with initial values: a(1)=120, a(2)=582,
 *            a(3)=2804, a(4)=13676, a(5)=66228, a(6)=324556
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 25, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Forward declaration of helper; only used in C mode. */
#ifndef __cplusplus
static void ensure_cache_capacity(idx_t n);
static seq_t *cache = NULL;
static size_t cache_cap = 0;
#endif

/* Number of terms computed so far (last index that has a stored value). */
static idx_t computed_until = 0;

#ifndef __cplusplus
/*--- C implementation of dynamic array -----------------------------------*/
static void ensure_cache_capacity(idx_t n)
{
    if (cache_cap < (size_t)n + 1) {
        seq_t *new_cache = (seq_t *)realloc(cache, ((size_t)n + 1) * sizeof(seq_t));
        if (!new_cache) abort();          /* unrecoverable allocation failure   */
        cache = new_cache;
        cache_cap = (size_t)n + 1;
    }
}
#endif

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined below offset          */

    /* Compute missing terms up to n.  Use different storage in C vs C++. */
    if (n > computed_until) {

#ifdef __cplusplus
        /*--- C++ path: use a std::vector for automatic construction ---*/
        static std::vector<seq_t> cache_vec;
        if (cache_vec.size() <= n)
            cache_vec.resize(n + 1);       /* default constructed elements */

        for (idx_t i = computed_until + 1; i <= n; ++i) {
            switch (i) {
                case 1: cache_vec[1] = (seq_t)120;     break;
                case 2: cache_vec[2] = (seq_t)582;     break;
                case 3: cache_vec[3] = (seq_t)2804;    break;
                case 4: cache_vec[4] = (seq_t)13676;   break;
                case 5: cache_vec[5] = (seq_t)66228;   break;
                case 6: cache_vec[6] = (seq_t)324556;  break;
                default: {
                    /* recurrence: a(n) = 5*a(n-1) + 24*a(n-2) - 130*a(n-3)
                                   + 36*a(n-4) + 80*a(n-5) - 32*a(n-6)     */
                    seq_t pos =   5 * cache_vec[i - 1]
                                + 24 * cache_vec[i - 2]
                                + 36 * cache_vec[i - 4]
                                + 80 * cache_vec[i - 5];
                    seq_t neg = 130 * cache_vec[i - 3] + 32 * cache_vec[i - 6];
                    cache_vec[i] = pos - neg;
                }
            }
        }
        computed_until = n;
        return cache_vec[n];

#else
        /*--- C path: raw pointer + realloc --------------------------------*/
        ensure_cache_capacity(n);
        for (idx_t i = computed_until + 1; i <= n; ++i) {
            switch (i) {
                case 1: cache[1] = (seq_t)120;     break;
                case 2: cache[2] = (seq_t)582;     break;
                case 3: cache[3] = (seq_t)2804;    break;
                case 4: cache[4] = (seq_t)13676;   break;
                case 5: cache[5] = (seq_t)66228;   break;
                case 6: cache[6] = (seq_t)324556;  break;
                default: {
                    seq_t pos =   5 * cache[i - 1]
                                + 24 * cache[i - 2]
                                + 36 * cache[i - 4]
                                + 80 * cache[i - 5];
                    seq_t neg = 130 * cache[i - 3] + 32 * cache[i - 6];
                    cache[i] = pos - neg;
                }
            }
        }
        computed_until = n;
        return cache[n];
#endif
    }

    /* Cached value exists */
#ifdef __cplusplus
    static std::vector<seq_t> cache_vec;  /* defined above, but visible here      */
    return cache_vec[n];
#else
    return cache[n];
#endif
}
