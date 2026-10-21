/*
 * A216773  Primes p such that x^56 = -2 has no solution mod p.
 * https://oeis.org/A216773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: primes p such that the congruence x^56 ≡ -2 (mod p) has no solution.
   In other words, -2 is not a 56‑th power residue modulo p.
   For a prime p, the test is
       (-2)^((p-1)/gcd(56, p-1)) ≢ 1  (mod p)
   if the above congruence fails, p is a term of the sequence. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd64.h"

/* sequence cache ----------------------------------------------- */
static seq_t *seq_cache = NULL;  /* array of found primes */
static size_t seq_len  = 0;      /* number of stored terms   */
static size_t seq_cap  = 0;      /* allocated capacity      */

/* prime list for trial division ------------------------------- */
static uint64_t *prime_list = NULL;
static size_t   prime_len  = 0;
static size_t   prime_cap  = 0;
static uint64_t last_checked = 2;  /* next odd candidate to test */

/* ---------------------------------------------------------------- */

/* modular exponentiation for 64‑bit arithmetic ------------------- */
static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1ULL) result = (uint64_t)((__uint128_t)result * base % mod);
        base = (uint64_t)((__uint128_t)base * base % mod);
        exp >>= 1ULL;
    }
    return result;
}

/* ----------------- dynamic array helpers ----------------------- */
static void seq_grow(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

static void primes_grow(size_t need)
{
    if (need <= prime_cap) return;
    size_t new_cap = prime_cap ? prime_cap << 1 : 8;
    while (new_cap < need) new_cap <<= 1;
    prime_list = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    prime_cap = new_cap;
}

/* trial division primality test using the stored prime list ------ */
static bool is_prime_uint64(uint64_t cand)
{
    if (cand < 2) return false;
    if (cand == 2) return true;
    if (cand % 2 == 0) return false;
    for (size_t i = 0; i < prime_len; ++i) {
        uint64_t p = prime_list[i];
        if ((__uint128_t)p * p > cand) break;
        if (cand % p == 0) return false;
    }
    return true;
}

/* Ensure we have at least n terms in seq_cache ------------------- */
static void ensure_n_terms(idx_t n)
{
    if ((idx_t)seq_len >= n) return;

    /* Start from the current counter.  Add prime 2 once.          */
    if (last_checked == 2) {
        primes_grow(prime_len + 1);
        prime_list[prime_len++] = 2;
        last_checked = 3;               /* next candidate */
    }

    for (; (idx_t)seq_len < n; /* increment inside */) {
        /* test current candidate for primality */
        if (is_prime_uint64(last_checked)) {
            uint64_t p = last_checked;

            /* add the prime to the list for future divisibility tests */
            primes_grow(prime_len + 1);
            prime_list[prime_len++] = p;

            if (p > 2) {  /* skip p=2, it's not a sequence member */
                uint64_t d   = gcd64(56ULL, p - 1ULL);
                uint64_t expo = (p - 1ULL) / d;
                uint64_t base = (p >= 2) ? (p - 2ULL) : 0ULL;  /* base = -2 mod p */
                uint64_t pow  = mod_pow_uint64(base, expo, p);
                if (pow != 1ULL) {
                    seq_grow(seq_len + 1);
                    seq_cache[seq_len++] = (seq_t)p;
                }
            }
        }
        /* Move to the next candidate.  If we passed 2, skip even numbers. */
        if (last_checked == 3) last_checked = 5;
        else last_checked += 2;
    }
}

/* ---------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, harmless placeholder */
    ensure_n_terms(n);
    return seq_cache[n - 1];        /* OEIS offset: a(1) is the first term */
}
