/*
 * A195324  Even numbers that cannot be represented as the sum of an odd prime
 *          and a safe prime (A005385).
 * https://oeis.org/A195324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th even number that cannot be written as an odd
 *            prime plus a safe prime (Safe prime = prime of the form 2*q + 1
 *            where q is prime)
 * Range:     u8 n <= 17, u16 n <= 7310, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include utilities */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"

/* Cache for already computed a(n) values. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_size = 0;

/* Current even number to test next. */
static seq_t next_candidate = 2;

/* List of already discovered safe primes (uint64_t). */
static uint64_t *safe_arr = NULL;
static size_t safe_cap = 0;
static size_t safe_cnt = 0;
static idx_t safe_last_checked = 5;

/* Ensure the list of safe primes contains all safe primes <= limit. */
static void ensure_safe_primes_upto(idx_t limit)
{
    /* Only extend if needed. */
    if (limit < safe_last_checked - 1) return;

    /* Allocate a bit larger array if necessary. */
    for (uint64_t cand = safe_last_checked; cand <= (uint64_t)limit; cand += 2) {
        if (!is_prime_small((seq_t)cand))
            continue;
        uint64_t q = (cand - 1) / 2;
        if (is_prime_small((seq_t)q)) {
            /* Append to the safe primes list. */
            if (safe_cnt == safe_cap) {
                safe_cap = safe_cap ? safe_cap * 2 : 8;
                safe_arr = (uint64_t *)realloc(safe_arr, safe_cap * sizeof(uint64_t));
            }
            safe_arr[safe_cnt++] = cand;
        }
    }
    safe_last_checked = (uint64_t)limit + 1;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* Undefined offset; behavior irrelevant. */

    /* Grow the cache if needed. */
    while (cache_size < (size_t)n) {
        seq_t cand = next_candidate;
        next_candidate += 2;  /* Next even number. */

        /* Ensure we know all safe primes up to `cand`. */
        if (cand > (seq_t)UINT64_MAX)
            return (seq_t)0;  /* Overflow guard: sequence not defined beyond 64‑bit. */
        ensure_safe_primes_upto((idx_t)cand);

        bool representable = false;
        uint64_t u_cand = (uint64_t)cand;

        /* Check for a representation p + s = cand with p odd prime, s safe prime. */
        for (size_t i = 0; i < safe_cnt && safe_arr[i] <= u_cand - 2; ++i) {
            uint64_t s = safe_arr[i];
            uint64_t p = u_cand - s;
            if (p >= 3 && (p & 1) && is_prime_small((seq_t)p)) {
                representable = true;
                break;
            }
        }

        if (!representable) {
            /* Store the new term. */
            if (cache_size == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 4;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = cand;
        }
    }

    return cache[(size_t)n - 1];
}
