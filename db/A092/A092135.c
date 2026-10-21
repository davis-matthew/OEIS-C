/*
 * A092135  Number of spanning trees with degrees 1 and 3 in S_5 X P_{4n+2}.
 * https://oeis.org/A092135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 154*a(n-1) - 4584*a(n-2) + 38664*a(n-3) - 3888*a(n-4),
   with a(1)=0, a(2)=1080, a(3)=164592, a(4)=20396448. */

#include <stddef.h>

/* Maximum index handled statically – enough for all n ≤ 10000. */
#define CACHE_MAX 20000

/* Static cache: 0‑based indices correspond to a(n+1). */
static seq_t cache[CACHE_MAX + 1];
static size_t cache_used = 0;

/* Initialise the first four terms once. */
static void init_cache(void) {
    if (cache_used) return;
    cache[0] = (seq_t)0;
    cache[1] = (seq_t)1080;
    cache[2] = (seq_t)164592;
    cache[3] = (seq_t)20396448;
    cache_used = 4;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* safety – not used per OEIS offset */

    init_cache();

    if (n <= cache_used) return cache[(size_t)n - 1];

    /* Compute missing terms up to n. */
    for (size_t i = cache_used; i < (size_t)n; ++i) {
        seq_t val = (seq_t)154 * cache[i - 1];
        val -= (seq_t)4584 * cache[i - 2];
        val += (seq_t)38664 * cache[i - 3];
        val -= (seq_t)3888 * cache[i - 4];
        cache[i] = val;
    }
    cache_used = (size_t)n;
    return cache[(size_t)n - 1];
}
