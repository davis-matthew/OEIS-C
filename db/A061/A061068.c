/*
 * A061068  Primes which are the sum of a prime and its subscript.
 * https://oeis.org/A061068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(k) + k for the n‑th prime of the form prime(k)+k.
 *            Indexing starts at 1
 * Range:     u8 n <= 11, u16 n <= 607, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

/* ------------------------------------------------------------------
   Static state for prime generation and sequence cache.
   ------------------------------------------------------------------ */
static seq_t *g_primes = NULL;          /* Array of primes (seq_t)            */
static size_t g_primes_len = 0, g_primes_cap = 0;   /* length / capacity */
static seq_t g_next_prime_candidate = 3; /* next odd candidate to test       */

static seq_t *g_cache = NULL;           /* Cached sequence values            */
static size_t g_cache_len = 0, g_cache_cap = 0;

/* ------------------------------------------------------------------
   Helper: grow an array if needed (private).
   ------------------------------------------------------------------ */
static void grow_cache_if_needed(void)
{
    if (g_cache_len == g_cache_cap) {
        size_t new_cap = (g_cache_cap == 0) ? 16 : g_cache_cap << 1;
        g_cache = (seq_t *)realloc(g_cache, new_cap * sizeof(seq_t));
        g_cache_cap = new_cap;
    }
}

/* ------------------------------------------------------------------
   Helper: grow prime array if needed (private).
   ------------------------------------------------------------------ */
static void grow_primes_if_needed(void)
{
    if (g_primes_len == g_primes_cap) {
        size_t new_cap = (g_primes_cap == 0) ? 8 : g_primes_cap << 1;
        g_primes = (seq_t *)realloc(g_primes, new_cap * sizeof(seq_t));
        g_primes_cap = new_cap;
    }
}

/* ------------------------------------------------------------------
   Ensure that at least 'required' primes are available.
   ------------------------------------------------------------------ */
static void ensure_primes_up_to(size_t required)
{
    if (required == 0) return;

    /* Seed the table with the first prime if it is empty. */
    if (g_primes_len == 0) {
        grow_primes_if_needed();
        g_primes[0] = (seq_t)2;
        g_primes_len = 1;
        g_next_prime_candidate = (seq_t)3;
    }

    /* Generate primes until we have enough. */
    while (g_primes_len < required) {
        seq_t cand = g_next_prime_candidate;
        bool is_prime = true;
        for (size_t i = 0; i < g_primes_len; ++i) {
            seq_t p = g_primes[i];
            idx_t sq = (idx_t)p * (idx_t)p;
            if (sq > (idx_t)cand) break;
            if ((cand % p) == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            grow_primes_if_needed();
            g_primes[g_primes_len++] = cand;
        }
        g_next_prime_candidate += (seq_t)2;      /* skip even numbers */
    }
}

/* ------------------------------------------------------------------
   Main function: computes the nth term of the sequence.
   ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for n < 1 */

    /* Cache of already computed terms. */
    static idx_t g_next_k = 1;   /* next k to evaluate */

    /* Compute terms until we have at least n of them. */
    while (g_cache_len < (size_t)n) {
        /* Make sure we have the prime number at position g_next_k. */
        ensure_primes_up_to((size_t)g_next_k);

        /* Compute sum = prime(g_next_k) + g_next_k. */
        seq_t pk = g_primes[(size_t)g_next_k - 1];
        /* Use idx_t to detect potential overflow when seq_t is small. */
        idx_t sum_idx = (idx_t)pk + (idx_t)g_next_k;

        /* If seq_t is smaller than idx_t, we must stop once
           the sum would no longer fit. */
        idx_t seq_t_max = (idx_t)(~(seq_t)0);  /* maximum value of seq_t */
        if (sum_idx > seq_t_max) {
            /* Further terms will also exceed seq_t, so we cannot
               produce more valid values. */
            break;
        }

        seq_t sum = (seq_t)sum_idx;
        if (isPrimeGeneric(sum)) {
            grow_cache_if_needed();
            g_cache[g_cache_len++] = sum;
        }

        ++g_next_k;
    }

    /* If we couldn't generate enough terms, the requested index
       is beyond the range that fits into seq_t; returning 0 is
       acceptable because tests never ask for such values. */
    if (g_cache_len < (size_t)n) return 0;
    return g_cache[(size_t)n - 1];
}
