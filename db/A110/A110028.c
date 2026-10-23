/*
 * A110028  Primes with a prime number of digits, all of them prime, that add
 *          up to a prime.
 * https://oeis.org/A110028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   is_prime_ull
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A110028: primes with a prime number of digits,
 * all of whose digits are prime, and whose digit sum is also prime.
 * The terms are listed in increasing order of value.
 * This implementation builds the sequence on demand and caches all
 * previously found terms.  Each successive prime length (prime number
 * of digits) is processed until enough terms have been generated.
 *
 * The algorithm is O(T) where T is the number of candidates examined,
 * roughly the total of 4^ℓ over the processed prime lengths.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_ull.h"

/* -------------------------------------------------------------------- */
/* Dynamic array helpers for seq_t storage.                             */
/* -------------------------------------------------------------------- */
static void ensure_capacity(size_t min_cap, seq_t **arr, size_t *cap) {
    if (*cap < min_cap) {
        size_t new_cap = (*cap == 0 ? 8 : *cap * 2);
        if (new_cap < min_cap) new_cap = min_cap;
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* -------------------------------------------------------------------- */
/* Global state – a cache of all generated terms.                       */
/* -------------------------------------------------------------------- */
static seq_t *cache       = NULL;
static size_t cache_cap   = 0;
static size_t cache_size  = 0;
static size_t length_idx  = 0;

/* Prime lengths we need to examine (2,3,5,7,11,13,17,19,23,...).    */
static const int prime_lengths[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47
};
static const size_t PRIME_LEN_COUNT = sizeof(prime_lengths) / sizeof(prime_lengths[0]);

/* Digits that are themselves prime. */
static const int DIGITS[4] = {2, 3, 5, 7};

/* -------------------------------------------------------------------- */
/* Recursive generator that builds all numbers of a given length.      */
/* -------------------------------------------------------------------- */
static void generate_len_recursive(int pos, int len, uint64_t cur, int sum) {
    if (pos == len) {
        /* Test primality of the numeric value and the sum of its digits. */
        if (is_prime_ull(cur) && is_prime_ull((uint64_t)sum)) {
            ensure_capacity(cache_size + 1, &cache, &cache_cap);
            cache[cache_size++] = (seq_t)cur;   /* cast is safe for arbitrary‑precision types */
        }
        return;
    }
    for (int i = 0; i < 4; ++i) {
        uint64_t next = cur * 10ULL + (uint64_t)DIGITS[i];
        int next_sum = sum + DIGITS[i];
        generate_len_recursive(pos + 1, len, next, next_sum);
    }
}

static void generate_len(int len) {
    generate_len_recursive(0, len, 0ULL, 0);
}

/* -------------------------------------------------------------------- */
/* Public sequence function.                                           */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined for offset < 1 */

    /* Generate terms until cache has at least n entries. */
    while (cache_size < (size_t)n) {
        if (length_idx >= PRIME_LEN_COUNT) {
            /* All desired lengths processed – sequence exhausted for requested n. */
            return 0;
        }
        int next_len = prime_lengths[length_idx++];
        generate_len(next_len);
    }
    return cache[n - 1];
}
