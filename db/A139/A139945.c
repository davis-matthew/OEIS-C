/*
 * A139945  Primes of the form 7x^2+51y^2.
 * https://oeis.org/A139945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p such that p = 7*x² + 51*y².
 *            Equivalently, p ≡ 7, 79, 163, 211, 235, 295, 379, 403, 415, 487,
 *            499, 547, 571, 583, 751, 823, 907, 991, 1051, 1159, 1219, 1255,
 *            1303, 1387, 1423 (mod 1428)
 * Range:     u8 n <= 4, u16 n <= 416, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/append_candidate.h"
#include "../utils/next_prime.h"

/* Residue classes modulo 1428 at which a prime can lie. */
static const uint16_t residues[25] = {
    7,   79,  163, 211, 235, 295, 379, 403, 415,
    487, 499, 547, 571, 583, 751, 823, 907, 991,
   1051,1159,1219,1255,1303,1387,1423
};
static const size_t residue_cnt = 25;

/* Return true iff p is one of the admissible residues modulo 1428. */
static inline bool is_residue_prime(uint64_t p) {
    uint64_t r = p % 1428ULL;
    for (size_t i = 0; i < residue_cnt; ++i)
        if (residues[i] == r) return true;
    return false;
}

/* Cache of primes satisfying the condition. */
static seq_t  *prime_cache = NULL;
static size_t  cache_len   = 0;   /* number of cached primes      */
static size_t  cache_cap   = 0;   /* current allocation capacity  */

/* Where to start the next prime search. */
static uint64_t next_start = 2;   /* initial candidate for next_prime */

/* Ensure at least 'n' elements are cached. */
static void ensure_cache(idx_t n) {
    while (cache_len < n) {
        /* Find the next prime >= next_start. */
        uint64_t p = next_prime(next_start);
        /* For the subsequent call, start from the next integer after p. */
        next_start = p + 1;

        if (is_residue_prime(p)) {
            /* Append to the cache (grow if necessary). */
            append_candidate(&prime_cache, &cache_len, &cache_cap, (seq_t)p);
        }
    }
}

/* External function required by the framework. */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* sequence is defined only for n >= 1 */
    ensure_cache(n);
    return prime_cache[n - 1];
}
