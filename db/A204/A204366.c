/*
 * A204366  Number of (n+2) X 6 0..2 arrays with every 3 X 3 subblock having
 *          three equal elements in a row horizontally, vertically or nw-to-se
 *          diagonally exactly three ways, and new values 0..2 introduced in
 *          row major order.
 * https://oeis.org/A204366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 6*a(n-4) + 4*a(n-6) (n > 9) * Initial values:
 *            * a(1)=891, a(2)=713, a(3)=622, a(4)=704, a(5)=1264, *
 *            a(6)=2956, a(7)=6772, a(8)=17852, a(9)=48260
 * Range:     u8 none, u16 n <= 9, u32 n <= 20, u64 n <= 42, u128 n <= 86,
 *            big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_alloc.h"

static const seq_t init[9] = {
    (seq_t)891,
    (seq_t)713,
    (seq_t)622,
    (seq_t)704,
    (seq_t)1264,
    (seq_t)2956,
    (seq_t)6772,
    (seq_t)17852,
    (seq_t)48260
};

static seq_t *cache = NULL;
static size_t cache_size = 0;   /* number of computed terms stored  (cache[i] = a(i+1)) */
static size_t cache_cap  = 0;   /* allocated capacity of cache */

/* Ensure the cache has room for at least 'needed' terms. */
static void
ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;

    seq_t *new_cache = seq_alloc(needed);   /* allocates and default‑constructs new elements */
    if (!new_cache)
        return; /* allocation failed – keep old cache (unlikely in test environment) */

    /* Copy the already computed terms */
    for (size_t i = 0; i < cache_size && i < needed; ++i)
        new_cache[i] = cache[i];

    cache = new_cache;
    cache_cap = needed;
}

seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n<1; return 0 as a safety value */

    size_t target = (size_t)n;    /* n is at most the range we need (<=10000) */

    ensure_capacity(target);

    /* Compute terms from cache_size+1 up to target */
    for (size_t i = cache_size + 1; i <= target; ++i) {
        seq_t val;
        if (i <= 9) {
            val = init[i - 1];
        } else {
            /* a(i) = 3*a(i-1) - 6*a(i-4) + 4*a(i-6) */
            val = 3 * cache[i - 2];      /* a(i-1) */
            val -= 6 * cache[i - 5];      /* a(i-4) */
            val += 4 * cache[i - 7];      /* a(i-6) */
        }
        cache[i - 1] = val;
    }

    cache_size = target;
    return cache[n - 1];
}
