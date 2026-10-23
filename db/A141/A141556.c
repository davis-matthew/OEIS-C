/*
 * A141556  Composites of the form c(p(n)) + p(c(n)), where c(n) = n-th
 *          composite and p(n) = n-th prime.
 * https://oeis.org/A141556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = c(p(n)) + p(c(n)) where * c(k) = k-th composite (c(1)=4),
 *            p(k) = k-th prime (p(1)=2). * The function returns the n-th term
 *            of the sequence. * It builds the prime list and composite list
 *            on demand and caches * results so that calls may appear in any
 *            order
 * Range:     u8 n <= 13, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   is_prime_generic
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_generic.h"   /* bool is_prime_generic(idx_t) */

static uint8_t  *prime_sieve      = NULL;   /* 1 = prime, 0 = composite */
static size_t   sieve_limit       = 0;      /* current size of the sieve */

static uint32_t *prime_list      = NULL;   /* 0‑based list of primes */
static size_t   prime_list_len   = 0;
static size_t   prime_list_cap   = 0;

static uint64_t *composite_arr   = NULL;   /* 0‑based list of composites */
static size_t   composite_len    = 0;
static size_t   composite_cap    = 0;
static uint64_t next_composite   = 4;      /* next integer to test */

static seq_t *seq_cache      = NULL;
static size_t seq_cache_len  = 0;
static size_t seq_cache_cap  = 0;

/* Add a prime to the list, growing the array if needed. */
static void add_prime(uint32_t val)
{
    if (prime_list_len == prime_list_cap) {
        size_t new_cap = prime_list_cap ? prime_list_cap * 2 : 8;
        uint32_t *tmp = (uint32_t *)realloc(prime_list, new_cap * sizeof(uint32_t));
        if (!tmp) abort();
        prime_list = tmp;
        prime_list_cap = new_cap;
    }
    prime_list[prime_list_len++] = val;
}

/* Rebuild sieve up to 'limit' and rebuild the prime list. */
static void rebuild_sieve(size_t limit)
{
    free(prime_sieve);
    prime_sieve = (uint8_t *)malloc(limit + 1);
    if (!prime_sieve) abort();
    memset(prime_sieve, 1, limit + 1);
    if (limit >= 0) prime_sieve[0] = 0;
    if (limit >= 1) prime_sieve[1] = 0;

    for (size_t i = 2; i * i <= limit; ++i) {
        if (prime_sieve[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                prime_sieve[j] = 0;
        }
    }

    /* rebuild prime list */
    prime_list_len = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (prime_sieve[i])
            add_prime((uint32_t)i);

    sieve_limit = limit;
}

/* Ensure the sieve covers at least 'n' (upper bound). */
static void ensure_sieve_limit(size_t n)
{
    if (sieve_limit >= n) return;
    size_t new_limit = sieve_limit ? sieve_limit * 2 : 1000;
    if (new_limit < n) new_limit = n;
    rebuild_sieve(new_limit);
}

/* Ensure we have at least 'need' primes in prime_list. */
static void ensure_prime_count(idx_t need)
{
    if (prime_list_len >= (size_t)need) return;

    while (prime_list_len < (size_t)need) {
        size_t new_limit = sieve_limit ? sieve_limit * 2 : 1000;
        size_t need_limit = (size_t)(need * 15) + 100;
        if (new_limit < need_limit) new_limit = need_limit;
        rebuild_sieve(new_limit);
    }
}

/* Ensure we have at least 'need' composites in composite_arr. */
static void ensure_composite_idx(idx_t need)
{
    while (composite_len < (size_t)need) {
        ensure_sieve_limit(next_composite);
        if (!prime_sieve[next_composite]) {   /* composite */
            if (composite_len == composite_cap) {
                size_t new_cap = composite_cap ? composite_cap * 2 : 8;
                uint64_t *tmp = (uint64_t *)realloc(composite_arr,
                                                    new_cap * sizeof(uint64_t));
                if (!tmp) abort();
                composite_arr = tmp;
                composite_cap = new_cap;
            }
            composite_arr[composite_len++] = next_composite;
        }
        ++next_composite;
    }
}

/* Return true if v is composite (i.e., not prime). */
static inline bool is_composite_seq(seq_t v)
{
    if (v <= (seq_t)1) return false;
    return !is_prime_generic((idx_t)v);
}

/* Grow the sequence cache if needed. */
static void maybe_grow_seq_cache(void)
{
    if (seq_cache_len < seq_cache_cap) return;
    size_t new_cap = seq_cache_cap ? seq_cache_cap * 2 : 8;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq_cache = tmp;
    seq_cache_cap = new_cap;
}

static idx_t global_test_k = 0;   /* current value of k being tested */

/* Main sequence generator. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined, but guard */

    /* compute terms until we have at least n cached */
    while (seq_cache_len < (size_t)n) {
        ++global_test_k;  /* next k to evaluate */

        /* Ensure data for index k */
        ensure_prime_count((idx_t)global_test_k);
        ensure_composite_idx((idx_t)global_test_k);

        uint64_t c_k = composite_arr[global_test_k - 1];    /* c(k)          */
        uint32_t p_k = prime_list[global_test_k - 1];      /* p(k)          */

        /* c(p(k)) */
        ensure_composite_idx((idx_t)p_k);
        uint64_t c_p_k = composite_arr[(size_t)p_k - 1];

        /* p(c(k)) */
        ensure_prime_count((idx_t)c_k);
        uint32_t p_c_k = prime_list[(size_t)c_k - 1];

        seq_t sum = (seq_t)c_p_k + (seq_t)p_c_k;

        if (is_composite_seq(sum)) {
            maybe_grow_seq_cache();
            seq_cache[seq_cache_len++] = sum;
        }
    }

    return seq_cache[n - 1];
}
