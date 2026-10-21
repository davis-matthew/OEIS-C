/*
 * A215208  Primes congruent to {2, 7} mod 17.
 * https://oeis.org/A215208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 803, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   extend_uint64_vector
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for primes ≡ 2 or 7 (mod 17); a(n) = n‑th such prime. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/extend_uint64_vector.h"

/* Storage for all primes found and for those with residue 2 or 7 mod 17. */
static uint64_t  *prime_list  = NULL;
static size_t    prime_len   = 0, prime_cap   = 0;
static uint64_t  *special_list = NULL;
static size_t    special_len = 0, special_cap = 0;
static uint64_t  last_checked = 1;   /* value that has been tested last */

/* Append a prime; if it satisfies the residue condition, store it in special_list. */
static void append_prime(uint64_t p) {
    extend_uint64_vector(&prime_list, &prime_cap, prime_len + 1);
    prime_list[prime_len++] = p;

    if (p % 17 == 2 || p % 17 == 7) {
        extend_uint64_vector(&special_list, &special_cap, special_len + 1);
        special_list[special_len++] = p;
    }
}

/* Simple trial division using the primes already found. */
static bool is_prime_candidate(uint64_t cand) {
    if (cand < 2) return false;
    for (size_t i = 0; i < prime_len; ++i) {
        uint64_t p = prime_list[i];
        if (p > cand / p) break;          /* p*p would overflow or exceed cand */
        if (cand % p == 0) return false;
    }
    return true;
}

/* Grow the special_list until it contains at least `n` elements. */
static void ensure_nth(idx_t n) {
    while ((idx_t)special_len < n) {
        ++last_checked;
        if (is_prime_candidate(last_checked)) {
            append_prime(last_checked);
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* safety; off‑by‑one guard */
    ensure_nth(n);
    seq_t result = special_list[n - 1];
    return result;
}
