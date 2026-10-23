/*
 * A049482  Primes p such that p + 210 is also prime.
 * https://oeis.org/A049482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 2798, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes p with p + 210 also prime by testing each successive prime. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/next_prime.h"
#include "../utils/is_prime.h"

static seq_t *seq_cache = NULL;
static size_t seq_len  = 0;
static size_t seq_cap  = 0;
static uint64_t current_candidate = 2;

static void ensure_cache(idx_t n)
{
    /* Allocate initial capacity if needed */
    if (seq_cache == NULL) {
        seq_cap = 8;
        seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    }

    while (seq_len < n) {
        uint64_t p = next_prime(current_candidate);
        current_candidate = p + 1;

        if (is_prime((idx_t)(p + 210))) {
            if (seq_len == seq_cap) {
                seq_cap *= 2;
                seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
            }
            seq_cache[seq_len++] = (seq_t)p;
        }
    }
}

seq_t a(idx_t n)
{
    /* n is 1-based index */
    ensure_cache(n);
    return seq_cache[n - 1];
}
