/*
 * A224686  Number of (n+4) X 8 0..1 matrices with each 5 X 5 subblock
 *          idempotent.
 * https://oeis.org/A224686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) - 2*a(n-3) + 3*a(n-4) - 2*a(n-6) +
 *            2*a(n-8) - a(n-9) for n > 13 a(1)=3465, a(2)=1720, a(3)=1896,
 *            a(4)=2166, a(5)=2429, a(6)=2646, a(7)=3214, a(8)=4122,
 *            a(9)=5307, a(10)=6699, a(11)=8253, a(12)=10313, a(13)=13224
 * Range:     u8 none, u16 n <= 18, u32 n <= 58, u64 n <= 137, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation keeps a static cache (size 10001) large enough
   to support all n up to about 10000.  For C++ builds seq_t may be an
   arbitrary‑precision integer class; for C builds it is a POD type. */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n<1, return 0 */

    /* Static cache of computed terms.  10001 gives room for up to n=10000. */
    static seq_t cache[10001];
    static bool initialized = false;
    static idx_t max_cached = 13;         /* highest index that has been computed */

    /* One‑time initialization of the first 13 terms. */
    if (!initialized) {
        cache[1] = 3465;
        cache[2] = 1720;
        cache[3] = 1896;
        cache[4] = 2166;
        cache[5] = 2429;
        cache[6] = 2646;
        cache[7] = 3214;
        cache[8] = 4122;
        cache[9] = 5307;
        cache[10] = 6699;
        cache[11] = 8253;
        cache[12] = 10313;
        cache[13] = 13224;
        initialized = true;
    }

    if (n <= max_cached)         /* already computed */
        return cache[n];

    /* compute up to n, extending the cache only once per call */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        /*
          a(i) = 3*a(i-1) - 2*a(i-2) - 2*a(i-3) + 3*a(i-4)
                 - 2*a(i-6) + 2*a(i-8) - a(i-9)
        */
        seq_t val = 3 * cache[i-1]
                  - 2 * cache[i-2]
                  - 2 * cache[i-3]
                  + 3 * cache[i-4]
                  - 2 * cache[i-6]
                  + 2 * cache[i-8]
                  -     cache[i-9];
        cache[i] = val;
    }
    max_cached = n;
    return cache[n];
}
