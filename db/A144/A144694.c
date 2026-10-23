/*
 * A144694  Initial terms from the linear congruential pseudorandom number
 *          generator defined by the recurrence a(n + 1) = a(n) * 7202161 +
 *          1234567 (modulo 277945762500) with a(0) = 0.
 * https://oeis.org/A144694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0; a(n+1) = (a(n) * 7202161 + 1234567) mod 277945762500 */

#include <stdlib.h>

static inline seq_t mul_mod_seq(seq_t a, seq_t b, seq_t mod)
{
    seq_t res = 0;
    while (b) {
        if (b & 1U) {
            res += a;
            if (res >= mod) res -= mod;
        }
        a <<= 1;
        if (a >= mod) a -= mod;
        b >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* constants of the recurrence */
    static const seq_t MOD     = (seq_t)277945762500ULL;
    static const seq_t FACTOR  = (seq_t)7202161ULL;
    static const seq_t ADDEND  = (seq_t)1234567ULL;

    /* Initialize cache for n = 0 if it hasn't been done yet */
    if (cache_size == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) exit(1);          /* abort on allocation failure */
        cache[0] = (seq_t)0;
        cache_size = 1;
    }

    /* If the requested index is already known, return it */
    if (n < cache_size)
        return cache[n];

    /* Increase cache size to accommodate up to n */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) exit(1);                  /* abort on allocation failure */
    cache = tmp;

    /* Compute missing terms iteratively */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t prod = mul_mod_seq(cache[i - 1], FACTOR, MOD);
        prod += ADDEND;
        if (prod >= MOD) prod -= MOD;
        cache[i] = prod;
    }

    cache_size = new_size;
    return cache[n];
}
