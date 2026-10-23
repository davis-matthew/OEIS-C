/*
 * A185970  a(n) = 2^((n^2-n-2)/2)*(n+2)!
 * https://oeis.org/A185970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^((n^2-n-2)/2)*(n+2)! (OEIS A185970)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* fast 2^e for seq_t (works for both built‑in and big‑int seq_t) */
static seq_t pow2_seq(idx_t e) {
    seq_t res = (seq_t)1;
    seq_t base = (seq_t)2;
    while (e) {
        if (e & 1) res *= base;
        base *= base;
        e >>= 1;
    }
    return res;
}

seq_t a(idx_t n) {
    /* cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of stored terms */

    /* initialise cache with a(0) = 1 */
    if (!cache) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* safety */
        cache[0] = (seq_t)1;              /* a(0) = 1 */
        cache_len = 1;
    }
    /* current cache contains terms [0 .. cache_len-1] */

    if (n < cache_len) return cache[n];

    /* enlarge cache if needed */
    cache = (seq_t*)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
    if (!cache) return (seq_t)0;          /* safety */
    /* fill missing values */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        /* recurrence: a(i) = a(i-1) * 2^(i-1) * (i+2) */
        seq_t prev = cache[i - 1];
        seq_t pow_val = pow2_seq((idx_t)(i - 1));
        seq_t mult = pow_val * (seq_t)(i + 2);
        cache[i] = prev * mult;
    }
    cache_len = (size_t)(n + 1);
    return cache[(size_t)n];
}
