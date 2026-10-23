/*
 * A047933  Consider primes p with least positive primitive root g such that
 *          q=p+g is next prime after p; sequence gives values of q.
 * https://oeis.org/A047933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 502, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factorize, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A047933:
   a(n) is the n-th prime q such that there is a prime p with
   least positive primitive root g and q = p + g is the next prime
   after p.  The sequence starts 3,5,7,13,31,...
   The algorithm builds the sequence lazily, caching results so that
   calls to a(n) in any order are O(1) amortised. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/factorize.h"

static inline uint64_t modpow_uint64(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1)
            result = (unsigned __int128) result * b % mod;
        b = (unsigned __int128) b * b % mod;
        exp >>= 1;
    }
    return result;
}

static uint64_t least_primitive_root(uint64_t p) {
    if (p == 2) return 1;                 // primitive root of 2 is 1
    uint64_t factors[64];
    int exps[64];
    size_t cnt = factorize(p - 1, factors, exps);   // unique prime factors of p-1
    for (uint64_t g = 2; ; ++g) {
        bool ok = true;
        for (size_t i = 0; i < cnt; ++i) {
            if (modpow_uint64(g, (p - 1) / factors[i], p) == 1) {
                ok = false;
                break;
            }
        }
        if (ok) return g;
    }
}

/* Cached sequence */
static seq_t *seq_cache = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;
static uint64_t next_prime_to_process = 2;   /* currently considered prime */

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined for 0, but safe */
    /* Compute until we have n terms */
    while (seq_len < (size_t)n) {
        uint64_t p = next_prime_to_process;
        uint64_t next_p = next_prime(p);  /* next prime after p */
        uint64_t g = least_primitive_root(p);
        uint64_t q = p + g;
        if (q == next_p) {   /* q is the next prime after p */
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 4;
                seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
            }
            seq_cache[seq_len++] = (seq_t)q;
        }
        next_prime_to_process = next_p;
    }
    return seq_cache[n - 1];
}
