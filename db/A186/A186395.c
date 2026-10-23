/*
 * A186395  Numbers k such that there are 3 primes between 100*k and 100*k +
 *          99.
 * https://oeis.org/A186395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 * Computes A186395: the nth integer k for which the block [100*k,100*k+99]
 * contains exactly 3 prime numbers.  
 * The function builds a binary sieve up to a fixed limit (200 000 000),
 * then scans successive centuries counting primes via the bit table.
 * It caches found k values so that subsequent calls are O(1).
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/grow_seq.h"

static inline int get_bit(const uint64_t *bits, uint64_t i) {
    return (bits[i >> 6] >> (i & 63)) & 1ULL;
}

static inline void clear_bit(uint64_t *bits, uint64_t i) {
    bits[i >> 6] &= ~(1ULL << (i & 63));
}

/* Global sieve data */
static uint64_t *prime_bits = NULL;
static uint64_t prime_limit = 0;

static void init_sieve(uint64_t limit) {
    if (prime_bits != NULL) {
        free(prime_bits);
    }
    uint64_t words = (limit >> 6) + 1;
    prime_bits = (uint64_t *)calloc(words, sizeof(uint64_t));
    for (uint64_t i = 0; i < words; ++i) {
        prime_bits[i] = ~0ULL; /* all bits set to 1 (prime candidates) */
    }
    /* 0 and 1 are not primes */
    clear_bit(prime_bits, 0);
    clear_bit(prime_bits, 1);

    for (uint64_t p = 2; p * p <= limit; ++p) {
        if (get_bit(prime_bits, p)) {
            for (uint64_t j = p * p; j <= limit; j += p) {
                clear_bit(prime_bits, j);
            }
        }
    }
    prime_limit = limit;
}

/* Cache of found k values */
static seq_t *k_cache = NULL;
static size_t k_cache_size = 0;
static size_t k_cache_cap = 0;
static uint64_t last_k_checked = 0;

seq_t a(idx_t n) {
    /* Ensure the sieve is initialized to a sufficiently large limit.
       200 000 000 numbers are more than enough for n up to 10 000. */
    if (prime_bits == NULL) {
        init_sieve(200000000ULL);
    }

    /* Extend the cache until we have n terms. */
    while (k_cache_size < (size_t)n) {
        ++last_k_checked;
        uint64_t start = last_k_checked * 100ULL;
        uint64_t end   = start + 99ULL;

        /* Count primes in the block [start, end] */
        int cnt = 0;
        for (uint64_t v = start; v <= end; ++v) {
            cnt += get_bit(prime_bits, v);
        }

        if (cnt == 3) {
            /* Grow cache if necessary */
            if (k_cache_size == k_cache_cap) {
                grow_seq(&k_cache, &k_cache_cap, k_cache_size + 1);
            }
            k_cache[k_cache_size++] = (seq_t)last_k_checked;
        }
    }
    return k_cache[n - 1];
}
