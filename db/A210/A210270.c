/*
 * A210270  Half the number of (n+1)X3 0..2 arrays with every 2X2 subblock
 *          having two or three distinct clockwise edge differences.
 * https://oeis.org/A210270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 20*a(n-1)+26*a(n-2)-364*a(n-3)-218*a(n-4)
 *            +1203*a(n-5)-418*a(n-6)-394*a(n-7) +216*a(n-8)-20*a(n-9) with
 *            initial values: a(1)=273, a(2)=5558, a(3)=113248, a(4)=2307956,
 *            a(5)=47036870, a(6)=958632858, a(7)=19537398722,
 *            a(8)=398181683100, a(9)=8115136633050
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache[n-1] holds a(n) for n>=1   */
static size_t cap   = 0;         /* current allocated capacity       */
static idx_t  max_n = 0;         /* largest n computed so far        */

/* Ensure that the cache can hold at least n elements (n >= 1). */
static void ensure_cache(idx_t n) {
    if (cap >= (size_t)n)
        return;
    size_t new_cap = cap ? cap << 1 : 1;
    while (new_cap < (size_t)n)
        new_cap <<= 1;
    seq_t *new_ptr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_ptr) abort();
    for (size_t i = 0; i < cap; ++i)
        new_ptr[i] = cache[i];
    free(cache);
    cache = new_ptr;
    cap   = new_cap;
}

/* Base values for n = 1 .. 9. */
static const seq_t init_vals[9] = {
    (seq_t)273,
    (seq_t)5558,
    (seq_t)113248,
    (seq_t)2307956,
    (seq_t)47036870,
    (seq_t)958632858,
    (seq_t)19537398722,
    (seq_t)398181683100,
    (seq_t)8115136633050
};

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0);          /* undefined for n < 1 – return 0 */
    else if (n == 1)      /* shortcut for very small n */
        return init_vals[0];
    /* First call – initialise cache. */
    if (!cache) {
        ensure_cache(9);
        for (idx_t i = 1; i <= 9; ++i)
            cache[i-1] = init_vals[i-1];
        max_n = 9;
    }
    /* Grow cache if necessary. */
    if ((size_t)n > cap)
        ensure_cache(n);
    /* Compute missing terms using the recurrence. */
    if (max_n < n) {
        for (idx_t k = max_n + 1; k <= n; ++k) {
            seq_t s = 0;
            /* Positive contributions */
            s += (seq_t)20  * cache[k-2];
            s += (seq_t)26  * cache[k-3];
            s += (seq_t)1203 * cache[k-6];
            s += (seq_t)216  * cache[k-9];
            /* Negative contributions */
            s -= (seq_t)364 * cache[k-4];
            s -= (seq_t)218 * cache[k-5];
            s -= (seq_t)418 * cache[k-7];
            s -= (seq_t)394 * cache[k-8];
            s -= (seq_t)20  * cache[k-10];
            cache[k-1] = s;
        }
        max_n = n;
    }
    return cache[n-1];
}
