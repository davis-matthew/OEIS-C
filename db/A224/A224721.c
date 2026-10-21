/*
 * A224721  Number of (n+3) X 4 0..2 matrices with each 4 X 4 subblock
 *          idempotent.
 * https://oeis.org/A224721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 12, u32 n <= 41, u64 n <= 99, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical linear recurrence for A224721
 * a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) + a(n-4)
 *        - 2*a(n-5) + 2*a(n-6) - 2*a(n-7) + a(n-8)   for n>11
 * Initial values:
 * a(1)=2052, a(2)=1623, a(3)=2471, a(4)=3506, a(5)=4257,
 * a(6)=5737, a(7)=8117, a(8)=12306, a(9)=18187,
 * a(10)=26561, a(11)=38040
 */

#include <stdlib.h>
#include <stdbool.h>

/* Helper to cast void* in C++ */
#ifdef __cplusplus
#define REINTERP_PTR(ptr, type) (reinterpret_cast< type >(ptr))
#else
#define REINTERP_PTR(ptr, type) ((type)(ptr))
#endif

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* array of computed terms */
    static idx_t cache_size = 0;         /* number of stored slots */
    static bool init = false;

    /* Initialise with the first 11 terms (indices 1..11, index 0 unused) */
    if (!init) {
        /* Allocate space for indices 0..11 */
        cache_size = 12;
        cache = REINTERP_PTR(malloc(cache_size * sizeof(seq_t)), seq_t*);
        if (!cache) abort();

        cache[0] = (seq_t)0;            /* unused placeholder */
        cache[1] = (seq_t)2052;
        cache[2] = (seq_t)1623;
        cache[3] = (seq_t)2471;
        cache[4] = (seq_t)3506;
        cache[5] = (seq_t)4257;
        cache[6] = (seq_t)5737;
        cache[7] = (seq_t)8117;
        cache[8] = (seq_t)12306;
        cache[9] = (seq_t)18187;
        cache[10] = (seq_t)26561;
        cache[11] = (seq_t)38040;

        init = true;
    }

    /* Grow cache if the requested index is beyond current capacity */
    if (n >= cache_size) {
        idx_t old_cap = cache_size;
        idx_t new_cap = n + 1;          /* ensure index n is valid */
        seq_t *tmp = REINTERP_PTR(realloc(cache, new_cap * sizeof(seq_t)), seq_t*);
        if (!tmp) abort();
        cache = tmp;
        cache_size = new_cap;

        /* Compute new terms using the recurrence */
        for (idx_t i = old_cap; i <= n; ++i) {
            if (i <= 11) continue;     /* initial values already set */
            seq_t val = (seq_t)3 * cache[i-1]
                        - (seq_t)3 * cache[i-2]
                        + cache[i-3]
                        + cache[i-4]
                        - (seq_t)2 * cache[i-5]
                        + (seq_t)2 * cache[i-6]
                        - (seq_t)2 * cache[i-7]
                        + cache[i-8];
            cache[i] = val;
        }
    }

    return cache[n];
}
