/*
 * A090456  Primes prime(k) having more binary 1's than k.
 * https://oeis.org/A090456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 5055, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, popcount_idx_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A090456: the n‑th prime p_k for which the binary weight of p_k
 *  exceeds that of k (k is the 1‑based index of p_k in the prime list). */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/popcount_idx_t.h"

/* popcount for a seq_t (may be an arbitrary precision type) */
static inline idx_t popcount_seq_t(seq_t x)
{
    idx_t c = 0;
    while (x) {
        if (x & 1) c++;
        x >>= 1;
    }
    return c;
}

/* static cache for qualifying primes */
static seq_t *cached_primes = NULL;
static size_t cached_cap   = 0;
static size_t cached_len   = 0;

/* sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence is 1‑based */

    /* Lazily extend the cache until it holds n elements */
    static idx_t next_candidate = 2; /* integer to test next */
    static idx_t prime_index   = 0;   /* 1‑based index of next_candidate if prime */

    while (cached_len < (size_t)n) {
        /* Move to the next prime */
        while (!is_prime(next_candidate)) ++next_candidate;
        ++prime_index;                 /* next_candidate is now the prime with index k */

        /* Check weight condition */
        seq_t p = (seq_t)next_candidate;
        if (popcount_seq_t(p) > popcount_idx_t(prime_index)) {
            /* store in cache */
            if (cached_len == cached_cap) {
                cached_cap = cached_cap ? cached_cap << 1 : 8;
                seq_t *tmp = (seq_t *)realloc(cached_primes, cached_cap * sizeof(seq_t));
                if (!tmp) abort();   /* allocation failed */
                cached_primes = tmp;
            }
            cached_primes[cached_len++] = p;
        }
        ++next_candidate;
    }

    return cached_primes[n - 1];
}
