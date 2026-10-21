/*
 * A095406  Numbers k such that Sum-of-digits-of-k <
 *          Sum-of-digits-of-all-distinct-prime-factors-of-k.
 * https://oeis.org/A095406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 96, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search‑based algorithm for OEIS sequence A095406:
   Numbers k such that digit_sum(k) < digit_sum(sum of distinct prime factors of k) */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ----- Helper: digit sum for an unsigned 64‑bit integer, cast to seq_t ----- */
static inline seq_t digit_sum_u64(uint64_t x)
{
    seq_t sum = 0;
    while (x) {
        sum += (seq_t)(x % 10);
        x /= 10;
    }
    return sum;
}

/* ----- Helper: factorize n and collect distinct prime factors (uint64_t) ----- */
static void factor_distinct(uint64_t n, uint64_t *primes, size_t *cnt)
{
    *cnt = 0;
    if (n < 2) {
        return;
    }
    /* handle factor 2 */
    if (n % 2 == 0) {
        primes[(*cnt)++] = 2;
        while (n % 2 == 0) n /= 2;
    }
    /* odd factors */
    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            primes[(*cnt)++] = p;
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) {
        primes[(*cnt)++] = n;
    }
}

/* ----- Cache of already found terms ----- */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_k = 2;   /* next integer to test */

/* ----- Ensure that at least n terms are cached ----- */
static void ensure_cache(idx_t n)
{
    while (cache_len < (size_t)n) {
        uint64_t k = next_k++;
        seq_t ds_k = digit_sum_u64(k);

        uint64_t primes[20];
        size_t cnt = 0;
        factor_distinct(k, primes, &cnt);

        seq_t ds_factors = 0;
        for (size_t i = 0; i < cnt; ++i) {
            ds_factors += digit_sum_u64(primes[i]);
        }

        if (ds_k < ds_factors) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)k;
        }
    }
}

/* ----- Public function: a(n) ----- */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;   /* undefined for n=0, return 0 as a safe fallback */
    }
    ensure_cache(n);
    return cache[(size_t)n - 1];
}
