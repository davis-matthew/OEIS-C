/*
 * A058999  Primes p such that x^17 = 2 has no solution mod p.
 * https://oeis.org/A058999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 390, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of the primes p for which the congruence
   x^17 ≡ 2 (mod p) has no solution.  For such a prime we must have
   p ≡ 1 (mod 17) and 2^((p-1)/17) ≠ 1 (mod p). */

/* External types are supplied by the build environment. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mod_pow.h"    /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */

/* Make sure a dynamic array has enough room. */
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size) {
    if (*cap >= needed) return;
    size_t new_cap = (*cap > 0) ? (*cap << 1) : 16;
    while (new_cap < needed) new_cap <<= 1;
    *ptr = realloc(*ptr, new_cap * elem_size);
    *cap = new_cap;
}

/* Small primality test suitable for 64‑bit numbers. */
static bool is_small_prime(uint64_t n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t p = 3; p * p <= n; p += 2)
        if (n % p == 0) return false;
    return true;
}

/* Caches of all primes seen and of the special primes. */
static uint64_t *primes = NULL, *specials = NULL;
static size_t primes_len = 0, specials_len = 0;
static size_t primes_cap = 0, specials_cap = 0;
static uint64_t next_candidate = 3;

/* Append a prime to the list of all primes. */
static void append_prime(uint64_t p) {
    ensure_capacity(primes_len + 1, &primes_cap, (void **)&primes, sizeof(uint64_t));
    primes[primes_len++] = p;
}

/* Append a special prime to the list of desired terms. */
static void append_special(uint64_t p) {
    ensure_capacity(specials_len + 1, &specials_cap, (void **)&specials, sizeof(uint64_t));
    specials[specials_len++] = p;
}

/* Return the nth term (1‑based).  Calls may come in any order. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;    /* undefined, but safe */
    while (specials_len < n) {
        uint64_t cand = next_candidate;
        next_candidate = cand + ((cand & 1) ? 2 : 1);  /* skip even numbers */
        if (!is_small_prime(cand)) continue;
        append_prime(cand);
        /* Only check primes with p ≡ 1 (mod 17). */
        if ((cand - 1) % 17 == 0) {
            uint64_t exp = (cand - 1) / 17;
            seq_t res = mod_pow((seq_t)2, exp, (seq_t)cand);
            if (res != (seq_t)1) append_special(cand);
        }
    }
    return (seq_t)specials[(size_t)n - 1];
}
