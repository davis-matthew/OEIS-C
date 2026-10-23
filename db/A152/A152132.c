/*
 * A152132  Maximal length of rook tour on an n X n+1 board.
 * https://oeis.org/A152132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) -3*a(n-2) +a(n-3) +a(n-4) -3*a(n-5) +3*a(n-6)
 *            -a(n-7) with initial values a(1)=2, a(2)=8, a(3)=24, a(4)=54,
 *            a(5)=104, a(6)=174, a(7)=270
 * Range:     u8 n <= 6, u16 n <= 45, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;           /* 1‑based cache */
static size_t cache_size = 0;         /* allocated slots (>=1) */
static size_t max_computed = 0;       /* highest n computed so far */
static int          initialized = 0;  /* flag: initial values loaded */

static const seq_t init[8] = {
    0,  /* unused index 0 */
    2,  /* a(1) */
    8,  /* a(2) */
    24,  /* a(3) */
    54,  /* a(4) */
    104, /* a(5) */
    174, /* a(6) */
    270  /* a(7) */
};

static void ensure_cache_size(size_t n)         /* need slots 0..n (inclusive) */
{
    if (n + 1 <= cache_size) return;
    size_t new_size = cache_size ? cache_size << 1 : 8;
    while (new_size <= n + 1) new_size <<= 1;
    seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_cache) return;                            /* allocation failure, caller must handle */
    if (cache) {
        for (size_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];
        free(cache);
    }
    cache = new_cache;
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                   /* undefined for n<1, return 0 for safety */

    if (!initialized) {
        ensure_cache_size(7);
        for (idx_t i = 1; i <= 7; ++i) cache[i] = init[i];
        max_computed = 7;
        initialized = 1;
    }

    if ((idx_t)max_computed >= n) return cache[n];

    ensure_cache_size((size_t)n);
    for (idx_t i = (idx_t)max_computed + 1; i <= n; ++i) {
        seq_t val = 3 * cache[i - 1] - 3 * cache[i - 2];
        val += cache[i - 3];
        val += cache[i - 4];
        val -= 3 * cache[i - 5];
        val += 3 * cache[i - 6];
        val -= cache[i - 7];
        cache[i] = val;
    }
    max_computed = n;
    return cache[n];
}
