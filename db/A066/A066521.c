/*
 * A066521  Primes p(k) such that the product of digits of p(k) equals the
 *          product of digits of k.
 * https://oeis.org/A066521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, less, nonn, changed
 * Formula:   Return the prime p(k) whose digit product equals that of its
 *            1‑based index k in the list of all primes. The sequence is built
 *            by sieving primes up to a fixed limit and keeping the qualifying
 *            ones in a cached array
 * Range:     u8 n <= 2, u16 n <= 37, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/digit_product.h"

static inline void append_prime(uint64_t **arr, size_t *len, size_t *cap, uint64_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 8 : (*cap << 1);
        uint64_t *newp = (uint64_t *)realloc(*arr, newcap * sizeof(uint64_t));
        if (!newp) return;          /* allocation failure – we silently abort */
        *arr   = newp;
        *cap   = newcap;
    }
    (*arr)[*len] = val;
    ++(*len);
}

seq_t a(idx_t n)
{
    /* static state ------------------------------------------------------ */
    static bool      initialized = false;
    static uint32_t *primes      = NULL;
    static size_t    primes_cnt  = 0;
    static uint64_t *prime_terms = NULL;
    static size_t    terms_len   = 0;
    static size_t    terms_cap   = 0;
    static idx_t     current_prime_idx = 0;   /* 0‑based index in primes[] */
    static idx_t     prime_ordinal    = 1;    /* 1‑based prime index */

    /* ------------------------------------------------------------------ */
    /* Initialise the sieve if this is the first call.                     */
    /* ------------------------------------------------------------------ */
    if (!initialized) {
        const uint64_t LIMIT = 20000000ULL;  /* enough for the first 10 000 terms */

        uint8_t *is_comp = (uint8_t *)calloc(LIMIT + 1, 1);
        if (!is_comp) return 0;  /* allocation failure */

        /* Sieve of Eratosthenes */
        for (uint64_t i = 2; i * i <= LIMIT; ++i) {
            if (!is_comp[i]) {
                for (uint64_t j = i * i; j <= LIMIT; j += i)
                    is_comp[j] = 1;
            }
        }

        /* Count primes */
        primes_cnt = 0;
        for (uint64_t i = 2; i <= LIMIT; ++i)
            if (!is_comp[i]) ++primes_cnt;

        /* Allocate array of primes */
        primes = (uint32_t *)malloc(primes_cnt * sizeof(uint32_t));
        if (!primes) {
            free(is_comp);
            return 0;
        }

        /* Fill primes array */
        size_t pos = 0;
        for (uint64_t i = 2; i <= LIMIT; ++i)
            if (!is_comp[i]) primes[pos++] = (uint32_t)i;

        free(is_comp);
        initialized = true;
    }

    /* ------------------------------------------------------------------ */
    /* Generate sequence terms until we have at least n of them.           */
    /* ------------------------------------------------------------------ */
    while (terms_len < n) {
        if (current_prime_idx >= primes_cnt) {
            /* In the unlikely event our limit is insufficient, we abort */
            return 0;
        }

        uint32_t p = primes[current_prime_idx++];
        seq_t prod_p = digit_product((seq_t)p);

        if (prod_p != 0) {
            seq_t prod_idx = digit_product((seq_t)prime_ordinal);
            if (prod_idx != 0 && prod_p == prod_idx) {
                append_prime(&prime_terms, &terms_len, &terms_cap, (uint64_t)p);
            }
        }

        ++prime_ordinal;
    }

    return (seq_t)prime_terms[n - 1];
}
