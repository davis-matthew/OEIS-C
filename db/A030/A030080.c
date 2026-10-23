/*
 * A030080  Primes p such that digits of p appear in p^3.
 * https://oeis.org/A030080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 85, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   big_pow, digit_frequencies, get_digits, is_prime_big,
 *            seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes OEIS A030080: primes p such that every digit of p
   occurs in p³.  It does so by testing primes in increasing order,
   storing the satisfying ones in a static cache, and returning the
   nth entry from that cache.  It works for any seq_t (built‑in or
   arbitrary precision) and any idx_t (uint64_t). */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"            /* bool is_prime_big(seq_t); */
#include "../utils/big_pow.h"                 /* seq_t big_pow(seq_t, seq_t); */
#include "../utils/digit_frequencies.h"       /* void digit_frequencies(seq_t, unsigned int[10]); */
#include "../utils/get_digits.h"              /* void get_digits(seq_t, uint8_t*, size_t*); */
#include "../utils/seq_realloc.h"             /* seq_t* seq_realloc(seq_t*, size_t, size_t); */

seq_t a(idx_t n)
{
    /* Static cache of primes satisfying the condition */
    static seq_t *cache      = NULL;   /* array of qualifying primes      */
    static size_t cache_cap  = 0;      /* allocated size of cache        */
    static size_t cache_len  = 0;      /* number of primes stored        */
    static seq_t last_checked = 1;     /* last prime examined            */

    /* Initialise the cache on the first call */
    if (cache == NULL) {
        cache_cap = 128;
        cache = seq_realloc(NULL, 0, cache_cap);
        cache_len = 0;
        last_checked = 1;
    }

    /* Generate primes until the cache holds at least n primes */
    while (cache_len < (size_t)n) {
        /* Find the next prime after last_checked */
        seq_t candidate = last_checked + (seq_t)1;
        while (!is_prime_big(candidate)) {
            candidate = candidate + (seq_t)1;
        }
        last_checked = candidate;

        /* Compute p³ */
        seq_t cube = big_pow(last_checked, (seq_t)3);

        /* Frequency table for digits of p³ */
        unsigned int freq_cube[10] = {0};
        digit_frequencies(cube, freq_cube);

        /* Digits of the prime p itself */
        uint8_t digits[32];
        size_t dlen;
        get_digits(last_checked, digits, &dlen);

        /* Check whether every digit of p appears in p³ */
        bool ok = true;
        for (size_t i = 0; i < dlen; ++i) {
            if (freq_cube[digits[i]] == 0) {
                ok = false;
                break;
            }
        }

        /* Store the prime if it satisfies the condition */
        if (ok) {
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap * 2;
                cache = seq_realloc(cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_len++] = last_checked;
        }
    }

    return cache[n - 1];
}
