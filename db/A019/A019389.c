/*
 * A019389  Primes with primitive root 66.
 * https://oeis.org/A019389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: generate primes p for which 66 is a primitive root modulo p */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime.h"
#include "../utils/powmod64.h"

/* Prime list used for factorisation of p-1 */
static uint64_t *prime_list = NULL;
static size_t   prime_cnt  = 0;
static size_t   prime_cap  = 0;

/* Cache of the sequence values found so far */
static seq_t *seq_cache = NULL;
static size_t seq_cnt    = 0;
static size_t seq_cap    = 0;

/* Next candidate integer to test (starts at 2) */
static uint64_t next_candidate = 2;

/* Ensure capacity of the prime list */
static void ensure_prime_list(size_t need)
{
    if (prime_cnt + need > prime_cap) {
        size_t new_cap = prime_cap ? prime_cap << 1 : 8;
        while (prime_cnt + need > new_cap) new_cap <<= 1;
        uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        prime_list = tmp;
        prime_cap  = new_cap;
    }
}

/* Ensure capacity of the sequence cache */
static void ensure_seq_cache(size_t need)
{
    if (seq_cnt + need > seq_cap) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 8;
        while (seq_cnt + need > new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq_cache = tmp;
        seq_cap   = new_cap;
    }
}

/* Test whether a prime p has 66 as a primitive root modulo p */
static bool has_primitive_root_66(uint64_t p)
{
    /* 66 must be coprime to p */
    if (p <= 3) return false;          /* p = 2 or 3 */
    if (66ULL % p == 0) return false;   /* p divides 66 (p = 11) */

    /* Factor p-1 using the list of known primes */
    uint64_t phi = p - 1;
    uint64_t tmp = phi;
    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= tmp; ++i) {
        uint64_t q = prime_list[i];
        if ((tmp % q) == 0) {
            while ((tmp % q) == 0) tmp /= q;
            if (powmod64(66ULL, (p - 1) / q, p) == 1ULL)
                return false;
        }
    }
    if (tmp > 1) { /* remaining prime factor */
        if (powmod64(66ULL, (p - 1) / tmp, p) == 1ULL)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n<1 */

    /* Generate terms until we have at least n of them */
    while (seq_cnt < (size_t)n) {
        uint64_t cand = next_candidate++;
        if (cand > 2 && (cand & 1ULL) == 0) continue;   /* skip even > 2 */
        if (!is_prime(cand)) continue;

        /* Store prime for future factorisations */
        ensure_prime_list(1);
        prime_list[prime_cnt++] = cand;

        if (has_primitive_root_66(cand)) {
            ensure_seq_cache(1);
            seq_cache[seq_cnt++] = (seq_t)cand;
        }
    }
    return seq_cache[n - 1];
}
