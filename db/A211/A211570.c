/*
 * A211570  Number of -2..2 arrays x(i) of n+1 elements i=1..n+1 with
 *          set{t,u,v in 0,1}((x[i+t]+x[j+u]+x[k+v])*(-1)^(t+u+v)) having
 *          three, four or five distinct values for every i,j,k<=n.
 * https://oeis.org/A211570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 5*a(n-2) - 5*a(n-3) - 6*a(n-4) + 6*a(n-5) with
 *            initial values a(1)=16, a(2)=28, a(3)=44, a(4)=74, a(5)=116
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 37, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

static void ensure_cap(size_t min_cap) {
    if (min_cap <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while (new_cap < min_cap) new_cap <<= 1;
    cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

static void init_cache(void) {
    if (cache_len >= 5) return;
    ensure_cap(6);                /* indexes 0..5 (0 unused) */
    cache[0] = 0;
    cache[1] = 16;
    cache[2] = 28;
    cache[3] = 44;
    cache[4] = 74;
    cache[5] = 116;
    cache_len = 5;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;        /* Undefined for offset-containing sequence */
    if (cache_len == 0) init_cache();

    /* Compute all missing values up to n */
    if (n > (idx_t)cache_len) {
        size_t up_to = (size_t)n;
        ensure_cap(up_to + 1);
        for (size_t i = cache_len + 1; i <= up_to; ++i) {
            seq_t val = cache[i-1];
            val += 5 * cache[i-2];
            val -= 5 * cache[i-3];
            val -= 6 * cache[i-4];
            val += 6 * cache[i-5];
            cache[i] = val;
        }
        cache_len = up_to;
    }

    return cache[(size_t)n];
}
