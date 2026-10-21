/*
 * A200842  Number of 0..n arrays x(0..6) of 7 elements without any two
 *          consecutive increases or two consecutive decreases.
 * https://oeis.org/A200842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 28*a(n-2) + 56*a(n-3) - 70*a(n-4) + 56*a(n-5)
 *            - 28*a(n-6) + 8*a(n-7) - a(n-8) initial values: a(1)=128,
 *            a(2)=1465, a(3)=8210, a(4)=31677, a(5)=96690, a(6)=250913,
 *            a(7)=577660, a(8)=1212729
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 30, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define MAX_N 10001   /* sufficient for n up to ~10000 */

static const seq_t init_vals[9] = {
    0,          /* unused, index 0 */
    128, 1465, 8210, 31677, 96690, 250913, 577660, 1212729
};

static seq_t cache[MAX_N];
static idx_t cache_size = 0;

/* ensure that cache contains values up to at least n */
static void ensure_cache(idx_t n)
{
    if (n <= cache_size) return;

    /* on first use, fill initial values */
    if (cache_size == 0) {
        /* may be necessary to initialize up to 8, but not exceeding max */
        idx_t limit = (n >= 8) ? 8 : (idx_t)n;
        for (idx_t i = 1; i <= limit; ++i) cache[i] = init_vals[i];
        cache_size = limit;
    }

    /* compute missing terms using the 8-term recurrence */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t pos = cache[i-1] * (seq_t)8
                  + cache[i-3] * (seq_t)56
                  + cache[i-5] * (seq_t)56
                  + cache[i-7] * (seq_t)8;
        seq_t neg = cache[i-2] * (seq_t)28
                  + cache[i-4] * (seq_t)70
                  + cache[i-6] * (seq_t)28
                  + cache[i-8];
        cache[i] = pos - neg;
    }

    cache_size = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                   /* not part of OEIS range */
    if (n >= (idx_t)MAX_N)                 /* arbitrary safe guard */
        return 0;                           /* out of precompiled range */

    if (n <= 8) return init_vals[n];

    ensure_cache(n);
    return cache[n];
}
