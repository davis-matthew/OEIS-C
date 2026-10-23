/*
 * A116843  Number of permutations of length n which avoid the patterns 1342,
 *          3241, 4312.
 * https://oeis.org/A116843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Number of permutations of length n avoiding patterns 1342, 3241,
 *            4312. Recurrence (derived from GF): for n >= 9, a(n) = 11*a(n-1)
 *            - 51*a(n-2) + 130*a(n-3) - 199*a(n-4) + 187*a(n-5) - 105*a(n-6)
 *            + 32*a(n-7) - 4*a(n-8) Initial values: a(1)=1, a(2)=2, a(3)=6,
 *            a(4)=21, a(5)=73, a(6)=241, a(7)=759, a(...
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>  /* for uint64_t used in idx_t */

#ifdef __cplusplus
using usize = std::size_t;
#else
typedef size_t usize;
#endif

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t max_computed = 0;

/* use unsigned __int128 for intermediate multiplication if available */
#ifdef __SIZEOF_INT128__
typedef unsigned __int128 u128;
#define USE_U128 1
#endif

static void ensure_cache(idx_t n)
{
    /* initialize cache if needed */
    if (!cache) {
        usize init = 9;   /* indices 0..8 */
        cache_size = init;
        max_computed = 8;
#ifdef __cplusplus
        cache = new seq_t[init];
#else
        cache = (seq_t*)malloc(init * sizeof(seq_t));
#endif
        /* initial terms (index 0 unused) */
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 2;
        cache[3] = 6;
        cache[4] = 21;
        cache[5] = 73;
        cache[6] = 241;
        cache[7] = 759;
        cache[8] = 2305;
    }

    /* grow cache array if necessary */
    if (n >= cache_size) {
        usize new_size = n + 1;
#ifdef __cplusplus
        seq_t *new_cache = new seq_t[new_size];
        for (idx_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];
        delete[] cache;
        cache = new_cache;
#else
        cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
#endif
        cache_size = new_size;
    }

    /* compute missing terms using recurrence */
    if (n > max_computed) {
#ifdef USE_U128
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            u128 pos = (u128)cache[i-1] * 11 +
                       (u128)cache[i-3] * 130 +
                       (u128)cache[i-5] * 187 +
                       (u128)cache[i-7] * 32;
            u128 neg = (u128)cache[i-2] * 51 +
                       (u128)cache[i-4] * 199 +
                       (u128)cache[i-6] * 105 +
                       (u128)cache[i-8] * 4;
            cache[i] = (seq_t)(pos - neg);
        }
#else
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t pos = cache[i-1] * 11 +
                        cache[i-3] * 130 +
                        cache[i-5] * 187 +
                        cache[i-7] * 32;
            seq_t neg = cache[i-2] * 51 +
                        cache[i-4] * 199 +
                        cache[i-6] * 105 +
                        cache[i-8] * 4;
            cache[i] = pos - neg;
        }
#endif
        max_computed = n;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* not used but safe */
    ensure_cache(n);
    return cache[n];
}
