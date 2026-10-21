/*
 * A216884  Primes p such that x^61 = 2 has a solution mod p.
 * https://oeis.org/A216884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A216884: primes p for which x^61 = 2 has a solution modulo p.
 * The sequence contains 2 and all odd primes p such that
 *   2^{(p-1)/gcd(61,p-1)} ≡ 1 (mod p).
 * For odd p, gcd(61,p-1)=61 exactly when p ≡ 1 (mod 61).  In that case this
 * test is performed; otherwise the condition is automatically satisfied. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"

static seq_t *seq = NULL;          /* array of sequence values (primes)   */
static size_t seq_len = 0, seq_cap = 0;

static uint64_t *prime_list = NULL;   /* primes used for primality tests    */
static size_t prime_cnt = 0, prime_cap = 0;

static uint64_t next_candidate = 2;   /* next integer to check              */

/* Append a value to the main sequence array. */
static void add_seq(uint64_t val) {
    if (seq_len == seq_cap) {
        seq_cap = seq_cap ? seq_cap << 1 : 4;
        seq = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
    }
    seq[seq_len++] = (seq_t)val;
}

/* Append a prime to the prime list. */
static void add_prime(uint64_t val) {
    if (prime_cnt == prime_cap) {
        prime_cap = prime_cap ? prime_cap << 1 : 4;
        prime_list = (uint64_t*)realloc(prime_list, prime_cap * sizeof(uint64_t));
    }
    prime_list[prime_cnt++] = val;
}

/* Basic trial‑division primality test using primes found so far. */
static bool is_prime(uint64_t n) {
    if (n < 2)          return false;
    if (n == 2)         return true;
    if ((n & 1u) == 0)   return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > n)   break;
        if (n % p == 0)  return false;
    }
    return true;
}

/* Generate the sequence until it contains at least n elements. */
static void generate_up_to(idx_t n) {
    while (seq_len < n) {
        uint64_t cand = next_candidate;
        /* find next prime */
        while (!is_prime(cand))
            ++cand;
        next_candidate = cand + 1;   /* next search will start after stages */

        bool ok = false;
        if (cand == 2) {
            ok = true;                     /* trivial solution exists   */
        } else if (cand % 61 != 1) {
            ok = true;                     /* gcd(61,p-1)=1 ⇒ always true */
        } else {
            /* cand % 61 == 1 : need 2^{(p-1)/61} ≡ 1 (mod p) */
            uint64_t exp = (cand - 1) / 61;
            ok = mod_pow((seq_t)2, exp, (seq_t)cand) == (seq_t)1;
        }

        if (ok)
            add_seq(cand);

        add_prime(cand);
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;      /* undefined for offset 1 but safe   */
    generate_up_to(n);
    return seq[n - 1];                 /* 1‑based indexing                 */
}
