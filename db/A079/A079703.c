/*
 * A079703  Numbers 4p where p is prime and 2p+1 and 4p+1 are composite.
 * https://oeis.org/A079703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4p, where p is prime and both 2p+1 and 4p+1 are composite
 * Range:     u8 n <= 5, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* --- Prime test (deterministic Miller–Rabin for 64‑bit integers) --- */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    /* small prime trial division */
    const uint64_t small_primes[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }

    /* write n-1 = d * 2^s  with d odd */
    uint64_t d = n - 1;
    uint64_t s = 0;
    while ((d & 1ULL) == 0ULL) {
        d >>= 1;
        ++s;
    }

    /* deterministic bases for 64‑bit numbers */
    const uint64_t bases[] = {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        __uint128_t x = 1, power = d, base = a;
        while (power) {
            if (power & 1ULL) x = (x * base) % n;
            base = (base * base) % n;
            power >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool witness = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) { witness = false; break; }
        }
        if (witness) return false;
    }
    return true;
}

/* --- Cache for computed terms --- */
static seq_t *terms = NULL;
static idx_t terms_len = 0;
static idx_t terms_cap = 0;

static void append_term(seq_t val)
{
    if (terms_len >= terms_cap) {
        terms_cap = terms_cap ? terms_cap * 2 : 32;
        terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
    }
    terms[terms_len++] = val;
}

/* --- Prime generation state --- */
static uint64_t next_candidate = 2;
static bool first_prime_done = false;

/* --- Main sequence function --- */
seq_t a(idx_t n)
{
    /* generate terms until we have at least (n+1) of them */
    while (terms_len <= n) {
        uint64_t p;

        if (!first_prime_done) {               /* handle the prime 2 first */
            p = 2;
            first_prime_done = true;
            next_candidate = 3;
        } else {
            uint64_t c = next_candidate;
            while (!is_prime_u64(c)) c += 2;   /* skip even numbers */
            p = c;
            next_candidate = c + 2;
        }

        /* check the composite conditions */
        if (!is_prime_u64(2 * p + 1) && !is_prime_u64(4 * p + 1)) {
            append_term((seq_t)(4 * p));
        }
    }

    return terms[n];
}
