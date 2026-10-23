/*
 * A107144  Primes of the form 5x^2 + 8y^2.
 * https://oeis.org/A107144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 822, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th prime that can be written in the form
   5*x^2 + 8*y^2.  Except for n = 1 (prime 5), all such primes are
   congruent to 13 or 37 modulo 40.  The function enumerates numbers of
   those two residue classes, tests them for primality using a
   deterministic Miller–Rabin test for 64‑bit integers, and collects
   them in order.  The result is returned as a seq_t value. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* 128‑bit multiplication modulo m (for m < 2^64). */
static uint64_t mul_mod_uint64(uint64_t a, uint64_t b, uint64_t m)
{
    return (unsigned __int128)a * b % m;
}

/* Modular exponentiation a^e mod m. */
static uint64_t pow_mod_uint64(uint64_t a, uint64_t e, uint64_t m)
{
    uint64_t r = 1;
    while (e) {
        if (e & 1) r = mul_mod_uint64(r, a, m);
        a = mul_mod_uint64(a, a, m);
        e >>= 1;
    }
    return r;
}

/* Deterministic Miller–Rabin primality test for 64‑bit numbers. */
static bool is_prime_64(uint64_t n)
{
    if (n < 2) return false;

    /* Small prime trial division. */
    static const uint64_t small_primes[12] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL,
        17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL
    };
    for (idx_t i = 0; i < 12; ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    /* Write n‑1 = d * 2^s with d odd. */
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    /* Deterministic bases for 64‑bit integers. */
    static const uint64_t bases[7] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL};
    for (idx_t i = 0; i < 7; ++i) {
        uint64_t a = bases[i];
        if (a % n == 0) return true;            /* a ≡ 0 mod n => n | a => n is prime */
        uint64_t x = pow_mod_uint64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (unsigned r = 1; r < s; ++r) {
            x = mul_mod_uint64(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)5;          /* the special prime 5 */

    /* Number of primes found so far (including 5). */
    idx_t found = 1;
    /* Index k for generating candidates of the form 13 + 40*k and 37 + 40*k. */
    idx_t k = 0;

    while (true) {
        /* Candidate 13 + 40*k */
        uint64_t cand = 13ULL + 40ULL * k;
        if (cand > 5 && is_prime_64(cand)) {
            ++found;
            if (found == n) return (seq_t)cand;
        }

        /* Candidate 37 + 40*k */
        cand = 37ULL + 40ULL * k;
        if (cand > 5 && is_prime_64(cand)) {
            ++found;
            if (found == n) return (seq_t)cand;
        }
        ++k;
    }
}
