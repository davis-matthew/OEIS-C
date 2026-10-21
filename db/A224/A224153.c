/*
 * A224153  Number of n X 3 0..1 arrays with diagonals and rows unimodal and
 *          antidiagonals nondecreasing.
 * https://oeis.org/A224153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) - a(n-3) - 4*a(n-4) + 14*a(n-5) -
 *            2*a(n-6) - 2*a(n-9) Initial values: a(1)=7, a(2)=28, a(3)=89,
 *            a(4)=281, a(5)=900, a(6)=2935, a(7)=9681, a(8)=32020,
 *            a(9)=105937
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 36, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t init_vals[9] = {
    7, 28, 89, 281, 900,
    2935, 9681, 32020, 105937
};

enum { MAX_CACHE = 20000 };          /* should cover all values requested (n <= 10000) */
static seq_t cache[MAX_CACHE + 1];    /* cache[n] holds a(n); cache[0] unused */
static idx_t cache_computed = 0;      /* largest n for which cache[n] is defined */

/* Ensure that cache has values up to n (inclusive).  Assumes n <= MAX_CACHE. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_computed)
        return;

    for (idx_t i = cache_computed + 1; i <= n; ++i)
    {
        if (i <= 9)
        {
            cache[i] = init_vals[i - 1];
        }
        else
        {
            seq_t v = 4 * cache[i - 1];
            v -= 2 * cache[i - 2];
            v -=   cache[i - 3];
            v -= 4 * cache[i - 4];
            v += 14 * cache[i - 5];
            v -= 2 * cache[i - 6];
            v -= 2 * cache[i - 9];
            cache[i] = v;
        }
    }
    cache_computed = n;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                 /* undefined for n < 1, but safe return */
    if (n > MAX_CACHE)
    {
        /* In practice we should handle larger n, but the problem guarantees n <= 10000. */
        /* If needed, extend the cache dynamically or compute via matrix exponentiation. */
        return 0;
    }
    ensure_cache(n);
    return cache[n];
}
