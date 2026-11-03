/*
 * A028882  Numbers k such that k^2 - 7 is prime.
 * https://oeis.org/A028882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 26, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 71 terms through n = 928 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for k such that k^2 - 7 is prime */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

#ifdef __cplusplus
# define SEQ_CONST(x) seq_t(x)
#else
# define SEQ_CONST(x) ((seq_t)(x))
#endif

seq_t a(idx_t n)
{
    /* Suppress warning for unused 0‑index */
    if (n == 0) return SEQ_CONST(0);

    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static idx_t  cache_size = 0;
    static seq_t  last_k = SEQ_CONST(0);

    /* First call: initialise the cache */
    if (cache == NULL)
    {
        cache_cap = 64;                  /* initial capacity – plenty for n≈10000 */
#ifdef __cplusplus
        cache = new seq_t[cache_cap];
#else
        cache = malloc(cache_cap * sizeof(seq_t));
#endif
        if (cache == NULL) exit(EXIT_FAILURE);
        cache[0] = SEQ_CONST(3);       /* first element */
        cache_size = 1;
        last_k = SEQ_CONST(3);
    }

    /* Return cached value if already computed */
    if (n - 1 < cache_size) return cache[n - 1];

    /* Extend cache until the desired index is reached */
    while (cache_size < n)
    {
        last_k = last_k + SEQ_CONST(1);
        seq_t val = last_k * last_k - SEQ_CONST(7);
        if (is_prime_big(val))
        {
            if (cache_size >= cache_cap)
            {
                size_t new_cap = cache_cap * 2;
#ifdef __cplusplus
                seq_t *new_cache = new seq_t[new_cap];
                for (size_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];
                delete[] cache;
#else
                seq_t *new_cache = realloc(cache, new_cap * sizeof(seq_t));
                if (new_cache == NULL) exit(EXIT_FAILURE);
#endif
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_size++] = last_k;
        }
    }

    return cache[n - 1];
}
