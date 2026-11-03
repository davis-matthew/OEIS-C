/*
 * A030080  Primes p such that digits of p appear in p^3.
 * https://oeis.org/A030080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 85, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 66 terms through n = 935 (2026-11-03)
 * Helpers:   digit_frequencies, get_digits, ipow, is_prime_u64
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
#include <stdlib.h>
#include "../utils/digit_frequencies.h"
#include "../utils/get_digits.h"
#include "../utils/ipow.h"
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

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

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
        seq_t cube = ipow(last_checked, (seq_t)3);

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
