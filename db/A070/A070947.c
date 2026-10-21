/*
 * A070947  Number of permutations on n letters that have only cycles of
 *          length 6 or less.
 * https://oeis.org/A070947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + (n-1)·a(n-2) + (n-1)(n-2)·a(n-3) * +
 *            (n-1)(n-2)(n-3)·a(n-4) + (n-1)(n-2)(n-3)(n-4)·a(n-5) * +
 *            (n-1)(n-2)(n-3)(n-4)(n-5)·a(n-6) * (for n ≥ 6, with a(0)=1,
 *            a(1)=1, a(2)=2, a(3)=6, a(4)=24, a(5)=120)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

static seq_t cache[10001];   /* buffer for computed values (index 0 … 10000) */
static idx_t cache_size = 0; /* number of elements already computed */

/* initialize the cache with the initial 6 factorial values */
static void init_cache(void) {
    if (cache_size != 0) return;
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)2;
    cache[3] = (seq_t)6;
    cache[4] = (seq_t)24;
    cache[5] = (seq_t)120;
    cache_size = 6;
}

/* compute a(n) using the recurrence and memoise results */
seq_t a(idx_t n) {
    init_cache();

    if (n < cache_size) return cache[n];          /* already known */

    if (n >= 10001) {
        /* if the build allows a larger sequence we could extend dynamically,
           but the problem guarantees n ≈ 10000, so we simply compute it
           up to the requested n without allocating more memory. */
        /* guard against out‑of‑range index */
        return (seq_t)0;
    }

    for (idx_t k = cache_size; k <= n; ++k) {
        if (k <= 5) {
            continue;   /* factorial values already set */
        }
        seq_t term = cache[k-1];   /* a(k-1) */
        seq_t prod = (seq_t)1;     /* product (k-1)*(k-2)*… */
        for (idx_t j = 2; j <= 6; ++j) {
            prod *= (seq_t)(k - j + 1);     /* multiply by (k-j+1) */
            term += prod * cache[k-j];     /* add contribution from a(k-j) */
        }
        cache[k] = term;
    }
    cache_size = n + 1;
    return cache[n];
}
