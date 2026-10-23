/*
 * A177371  Expansion of
 *          (1+12*x-24*x^2+8*x^4)/((1-8*x+4*x^2+4*x^3)*(1+2*x-2*x^2)).
 * https://oeis.org/A177371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) + 14*a(n-2) - 28*a(n-3) + 8*a(n-5) with initial
 *            values: a(0)=1, a(1)=18, a(2)=98, a(3)=812, a(4)=5748 The
 *            function is memoised; any call order is supported
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;   /* memoisation array */
static idx_t seq_cache_size = 0;  /* current allocated size */

/* initialise cache with the first five terms */
static void init_cache(void) {
    if (seq_cache != NULL)
        return;
    seq_cache_size = 5;
    seq_cache = (seq_t*)malloc(seq_cache_size * sizeof(seq_t));
    seq_cache[0] = 1;
    seq_cache[1] = 18;
    seq_cache[2] = 98;
    seq_cache[3] = 812;
    seq_cache[4] = 5748;
}

seq_t a(idx_t n) {
    init_cache();

    if (n < seq_cache_size)
        return seq_cache[n];

    /* grow cache to accommodate n */
    size_t new_size = (size_t)(n + 1);
    seq_t *new_cache = (seq_t*)realloc(seq_cache, new_size * sizeof(seq_t));
    if (!new_cache) { /* allocation failed – return 0 as a fallback */
        return 0;
    }
    seq_cache = new_cache;

    /* compute missing terms */
    for (idx_t i = seq_cache_size; i <= n; ++i) {
        seq_t prev1 = seq_cache[i - 1];
        seq_t prev2 = seq_cache[i - 2];
        seq_t prev3 = seq_cache[i - 3];
        seq_t prev5 = seq_cache[i - 5];
        seq_t val = prev1 * 6 + prev2 * 14 - prev3 * 28 + prev5 * 8;
        seq_cache[i] = val;
    }
    seq_cache_size = new_size;
    return seq_cache[n];
}
