/*
 * A178954  Primes prime(j) which cannot be written as 2*prime(j) = prime(j+k)
 *          + prime(j-k) for any 0 < k < j.
 * https://oeis.org/A178954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 988, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A178954: 
   primes p_j for which there is no k (0 < k < j) such that
   2*p_j = p_{j-k} + p_{j+k}. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/sieve_primes.h"

/* Global state for caching primes and sequence terms */
static idx_t *g_primes = NULL;      /* array of primes (idx_t = uint64_t)   */
static size_t g_prime_count = 0;    /* number of primes in g_primes          */
static seq_t *g_seq_cache = NULL;   /* cache of sequence terms               */
static size_t g_seq_len = 0;        /* number of cached terms                */
static size_t g_seq_cap = 0;        /* capacity of g_seq_cache               */
static size_t g_last_checked_j = 0; /* next prime index to test              */

/* Initialize prime array and sequence cache */
static void init_cache(void)
{
    if (g_primes != NULL)
        return;                                 /* already initialized */

    /* initial sieve limit – adjusted if more primes are needed later */
    size_t limit = 2000000;
    size_t cnt;
    idx_t *tmp;
    sieve_primes(limit, &tmp, &cnt);
    g_primes = tmp;
    g_prime_count = cnt;

    g_seq_cap = 32;
    g_seq_cache = (seq_t *)malloc(g_seq_cap * sizeof(seq_t));
    g_seq_len = 0;
}

/* Grow prime array to at least new_limit */
static void extend_primes(size_t new_limit)
{
    /* free old primes and re-sieve with larger limit */
    free(g_primes);
    size_t cnt;
    idx_t *tmp;
    sieve_primes(new_limit, &tmp, &cnt);
    g_primes = tmp;
    g_prime_count = cnt;
}

/* Add next prime that satisfies the condition to the cache */
static void test_next_prime(void)
{
    /* Ensure we have enough primes */
    if (g_last_checked_j >= g_prime_count) {
        /* request a larger limit (double the previous maximum) */
        size_t new_limit = (g_prime_count + 1) * 2;
        extend_primes(new_limit);
    }

    /* Test prime at index j = g_last_checked_j */
    size_t j = g_last_checked_j++;
    seq_t two_p = (seq_t)g_primes[j] * 2;
    bool ok = true;
    for (size_t k = 1; k < j; ++k) {
        size_t left  = j - k;
        size_t right = j + k;
        /* right < g_prime_count guaranteed after possible extension */
        seq_t sum = (seq_t)g_primes[left] + (seq_t)g_primes[right];
        if (sum == two_p) {
            ok = false;
            break;
        }
    }

    if (ok) {
        if (g_seq_len == g_seq_cap) {
            g_seq_cap *= 2;
            g_seq_cache = (seq_t *)realloc(g_seq_cache,
                                          g_seq_cap * sizeof(seq_t));
        }
        g_seq_cache[g_seq_len++] = (seq_t)g_primes[j];
    }
}

/* External function required by the harness */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n=0, guard against bad input */

    init_cache();

    /* Generate terms until we have at least n */
    while (g_seq_len < (size_t)n) {
        test_next_prime();
    }

    return g_seq_cache[(size_t)n - 1];
}
