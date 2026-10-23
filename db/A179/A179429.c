/*
 * A179429  Primes that are sum of three positive Fibonacci numbers.
 * https://oeis.org/A179429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   primes that can be written as the sum of three positive
 *            Fibonacci numbers
 * Range:     u8 n <= 38, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"

static seq_t *prime_cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static seq_t *fib = NULL;
static size_t fib_cnt = 0;
static size_t fib_cap = 0;

/* Ensure the Fibonacci array has at least 'req' slots. */
static void ensure_fib_cap(size_t req) {
    if (fib_cap < req) {
        size_t new_cap = fib_cap ? fib_cap << 1 : 64;
        while (new_cap < req) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(fib, new_cap * sizeof(seq_t));
        fib = tmp;
        fib_cap = new_cap;
    }
}

/* Ensure the prime result cache has at least 'req' slots. */
static void ensure_cache_cap(size_t req) {
    if (cache_cap < req) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 64;
        while (new_cap < req) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
        prime_cache = tmp;
        cache_cap = new_cap;
    }
}

/* Binary search for 'val' in the Fibonacci array. */
static bool fib_contains(seq_t val) {
    size_t lo = 0, hi = fib_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (fib[mid] == val) return true;
        if (fib[mid] < val)
            lo = mid + 1;
        else
            hi = mid;
    }
    return false;
}

/* Return the n-th prime that is a sum of three positive Fibonacci numbers. */
seq_t a(idx_t n) {
    static uint64_t candidate = 2;
    static bool fib_initialized = false;

    /* Initialize Fibonacci array once. */
    if (!fib_initialized) {
        ensure_fib_cap(2);
        fib[0] = (seq_t)1;
        fib[1] = (seq_t)1;
        fib_cnt = 2;
        fib_initialized = true;
    }

    /* Keep filling the cache until it contains the n-th term. */
    while (cache_len < n) {
        seq_t cand_seq = (seq_t)candidate;

        /* Check primality. */
        if (is_prime_small(cand_seq)) {
            /* Extend Fibonacci array up to the candidate prime. */
            while (fib[fib_cnt - 1] < cand_seq) {
                seq_t next = fib[fib_cnt - 1] + fib[fib_cnt - 2];
                ensure_fib_cap(fib_cnt + 1);
                fib[fib_cnt++] = next;
            }

            /* Test whether cand_seq can be expressed as a sum of three Fibonacci numbers. */
            bool representable = false;
            for (size_t i = 0; i < fib_cnt && fib[i] <= cand_seq - (seq_t)2; ++i) {
                seq_t fi = fib[i];
                for (size_t j = i; j < fib_cnt && fi + fib[j] <= cand_seq - (seq_t)1; ++j) {
                    seq_t sum2 = fi + fib[j];
                    seq_t rem   = cand_seq - sum2;
                    if (fib_contains(rem)) {
                        representable = true;
                        break;
                    }
                }
                if (representable) break;
            }

            /* Store the result if it satisfies the condition. */
            if (representable) {
                ensure_cache_cap(cache_len + 1);
                prime_cache[cache_len++] = cand_seq;
            }
        }
        candidate++;
    }

    return prime_cache[n - 1];
}
