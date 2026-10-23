/*
 * A154875  Numbers k such that k^4 contains every digit exactly 3 times.
 * https://oeis.org/A154875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 172, u64 n <= 172, u128 n <= 172,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) = the n‑th integer k such that k^4 has 30 decimal digits
   and each digit 0‑9 occurs exactly 3 times. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Helper code: compute 10^e as __uint128_t */
static __uint128_t pow10_u128(uint32_t e)
{
    __uint128_t r = 1;
    for (uint32_t i = 0; i < e; ++i)
        r *= 10;
    return r;
}

/* Cache of found terms */
static seq_t *cache = NULL;          /* dynamic array of terms */
static size_t cache_len = 0;         /* number of terms stored */
static size_t cache_cap = 0;         /* capacity of cache array */

/* Current integer being examined for the property */
static unsigned long long cur_k = 1; /* start at 1, loop until found */

/* precomputed limits 10^29 and 10^30 as unsigned __int128 */
static __uint128_t lim29 = 0, lim30 = 0;

/* ensure cache is large enough to hold n terms */
static void ensure_cache(size_t n)
{
    if (n <= cache_len)
        return;
    if (cache_cap == 0) {
        cache_cap = 1024;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
    }
    while (cache_len < n) {
        if (cache_len == cache_cap) {
            cache_cap *= 2;
            cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            if (!cache) abort();
        }
        /* calculate k^4 */
        __uint128_t t = (__uint128_t)cur_k;
        t = t * t * t * t;                 /* k^4 */

        /* check 30-digit length */
        if (t >= lim29 && t < lim30) {
            /* count digits */
            unsigned int cnt[10] = {0};
            __uint128_t tmp = t;
            for (int i = 0; i < 30; ++i) {
                unsigned int d = (unsigned int)(tmp % 10);
                cnt[d]++;
                tmp /= 10;
            }
            bool ok = true;
            for (int d = 0; d < 10; ++d)
                if (cnt[d] != 3) { ok = false; break; }
            if (ok) {
                cache[cache_len++] = (seq_t)cur_k;
            }
        }
        ++cur_k;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset less than 1 undefined */
    if (lim29 == 0) {      /* first time: compute limits */
        lim29 = pow10_u128(29);
        lim30 = lim29 * 10;
    }
    ensure_cache((size_t)n);
    return cache[n-1];
}
