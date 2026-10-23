/*
 * A008249  Coordination sequence T1 for Banalsite.
 * https://oeis.org/A008249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(12*m + k) = 408*m^2 + 68*k*m + d_k, 0 ≤ k < 12 where d_0..d_11
 *            = {1,4,11,22,42,67,97,133,176,223,277,335}. Recurrence for n ≥
 *            12: a(n) = a(n-1) + a(n-3) - a(n-5) - a(n-7) + a(n-8)
 * Range:     u8 n <= 9, u16 n <= 152, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Author: ChatGPT */

#include <stdlib.h>
/* seq_t and idx_t are defined in the build environment. */

static const seq_t init_vals[12] = {
    1, 4, 11, 22, 42, 67, 97, 133, 176, 223, 277, 335
};

static seq_t *cache = NULL;           /* dynamic cache of computed values     */
static idx_t  cached_max = (idx_t)-1; /* largest n for which cache[n] is valid */
static size_t buf_size = 0;           /* current allocated buffer size       */

/* allocate or enlarge cache to at least (n+1) elements */
static void ensure_capacity(idx_t n)
{
    if (cache == NULL) {
        buf_size = 12;
        cache = (seq_t*)malloc(buf_size * sizeof(seq_t));
        if (!cache) return; /* allocation failure – caller will handle (likely abort) */
        for (size_t i = 0; i < 12; ++i) cache[i] = init_vals[i];
        cached_max = 11;
        return;
    }
    if ((size_t)n + 1 <= buf_size) return;
    size_t new_size = buf_size;
    while (new_size <= (size_t)n) new_size <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return; /* allocation failure – caller will handle */
    cache = tmp;
    buf_size = new_size;
}

/* Compute a(n) using the recurrence, extending cache as needed */
seq_t a(idx_t n)
{
    ensure_capacity(n);
    if (n <= cached_max) return cache[n];

    for (idx_t i = cached_max + 1; i <= n; ++i) {
        cache[i] = cache[i-1] + cache[i-3] - cache[i-5] - cache[i-7] + cache[i-8];
    }
    cached_max = n;
    return cache[n];
}
