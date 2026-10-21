/*
 * A221134  Number of nX3 arrays of occupancy after each element moves to some
 *          horizontal or vertical neighbor, with no occupancy greater than 2.
 * https://oeis.org/A221134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 21*a(n-1) -1450*a(n-3) +4782*a(n-4) -372*a(n-5)
 *            +1450*a(n-6) +69*a(n-8) +a(n-9) Initial values: a(1)=2, a(2)=49,
 *            a(3)=656, a(4)=12544, a(5)=200072, a(6)=3485689, a(7)=58132316,
 *            a(8)=990486784, a(9)=16699002692
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

static seq_t *cache = NULL;          /* 1‑based array of computed terms   */
static idx_t cache_size = 0;         /* number of allocated slots         */
static bool initialized = false;     /* have we seeded the first 9 terms? */

/* Ensure that the cache can hold terms up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (!initialized) {
        /* Allocate at least n+1 slots (0 unused) and fill the first 9 values. */
        idx_t sz = (n >= 9) ? (n + 1) : 10;
        cache = (seq_t*)malloc(sz * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 2;
        cache[2] = 49;
        cache[3] = 656;
        cache[4] = 12544;
        cache[5] = 200072;
        cache[6] = 3485689;
        cache[7] = 58132316;
        cache[8] = 990486784;
        cache[9] = 16699002692ULL;
        cache_size = sz;
        initialized = true;
        return;
    }

    if (n < cache_size) return;

    size_t new_size = n + 1;
    if (new_size < cache_size * 2) new_size = cache_size * 2;
    cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    cache_size = new_size;
}

/* Compute a(n).  The function is safe for arbitrary order of calls. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; n>=1 in normal use */

    ensure_cache(n);

    /* For n <= 9 we already have the values. */
    if (n <= 9) return cache[n];

    /* Compute missing terms up to n, using the recurrence. */
    static idx_t last_computed = 9;   /* highest index computed so far */
    for (idx_t k = last_computed + 1; k <= n; ++k) {
        seq_t t = 21 * cache[k-1];
        t -= 1450 * cache[k-3];
        t += 4782 * cache[k-4];
        t -= 372  * cache[k-5];
        t += 1450 * cache[k-6];
        t += 69   * cache[k-8];
        t +=       cache[k-9];
        cache[k] = t;
    }
    last_computed = n;
    return cache[n];
}
