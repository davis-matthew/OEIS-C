/*
 * A074969  Numbers with six distinct prime divisors.
 * https://oeis.org/A074969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers with six distinct prime divisors.
   This implementation generates terms by sieving smallest prime factors
   up to a fixed limit and collecting all numbers that have exactly
   six distinct prime factors. Each call returns the n‑th such number
   (1‑based).  No explicit OEIS formula is used. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Resize the cached sequence array if needed.
   Works in both C and C++. */
static void grow_cache(seq_t **cache, size_t *cap, size_t needed)
{
    if (needed <= *cap)
        return;
    size_t new_cap = (*cap) ? (*cap) * 2 : 16;
    if (new_cap < needed)
        new_cap = needed;
    /* realloc works for malloced memory; in C++ this free/allocate is valid. */
    void *tmp = (void *)realloc(*cache, new_cap * sizeof(seq_t));
    *cache = (seq_t *)tmp;
    *cap   = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    static bool     init      = false;
    static seq_t  * cache     = NULL;
    static size_t    cache_len = 0;
    static size_t    cache_cap = 0;

    if (!init) {
        init = true;

        /* Choose a limit comfortably containing the first 10 000 terms.
           Adjust if higher n is needed. */
        const idx_t LIMIT = 20000000;

        /* Allocate tables for smallest prime factor and distinct‑prime counter. */
        uint32_t *spf = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
        uint8_t  *cnt = (uint8_t  *)malloc((LIMIT + 1) * sizeof(uint8_t));

        /* Initialize spf. */
        for (idx_t i = 0; i <= LIMIT; ++i)
            spf[i] = (uint32_t)i;

        /* Sieve to compute spf. */
        for (idx_t i = 2; i * i <= LIMIT; ++i) {
            if (spf[i] == (uint32_t)i) {
                for (idx_t j = i * i; j <= LIMIT; j += i) {
                    if (spf[j] == (uint32_t)j)
                        spf[j] = (uint32_t)i;
                }
            }
        }

        /* Count distinct prime factors and collect terms with count == 6. */
        cnt[0] = cnt[1] = 0;
        grow_cache(&cache, &cache_cap, 10000); /* initial capacity */

        for (idx_t i = 2; i <= LIMIT; ++i) {
            uint8_t c;
            if (spf[i] == (uint32_t)i) {          /* i is prime */
                c = 1;
            } else {
                idx_t m = i / spf[i];
                if (spf[m] == spf[i])             /* same prime factor as previous */
                    c = cnt[m];
                else
                    c = cnt[m] + 1;
            }
            cnt[i] = c;

            if (c == 6) {
                if (cache_len >= cache_cap)
                    grow_cache(&cache, &cache_cap, cache_len + 1);
                cache[cache_len++] = (seq_t)i;
            }
        }

        free(spf);
        free(cnt);
    }

    if (n < 1 || (size_t)n > cache_len)
        return (seq_t)0;
    return cache[n - 1];
}
