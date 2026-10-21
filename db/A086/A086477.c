/*
 * A086477  Primes p such that 128p+1 and (p-1)/128 are both prime.
 * https://oeis.org/A086477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A086477
 *  a(n) = the nth prime p such that 128p+1 and (p-1)/128 are both prime.
 *
 *  Formula: a(n) = the n‑th prime p with p≡1 (mod 128), p prime, and 128p+1 prime.
 *
 *  Implementation: iterate over the primes q=(p−1)/128, compute p=128q+1,
 *  test that p and 128p+1 are prime, and cache the found values.
 *
 *  The code uses a deterministic Miller–Rabin test for 64‑bit numbers.
 *  It is fully compliant with the requirements for any seq_t and idx_t
 *  definitions supplied by the build system.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* ----  deterministic Miller–Rabin for 64‑bit integers  ---- */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    /* Check small primes */
    static const uint64_t small_primes[] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL,
                                            13ULL, 17ULL, 19ULL, 23ULL, 29ULL,
                                            31ULL, 37ULL, 41ULL, 43ULL, 47ULL};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }

    /* Write n-1 as d*2^s with d odd */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0ULL) {
        d >>= 1;
        ++s;
    }

    /* Miller–Rabin bases sufficient for 64‑bit integers */
    static const uint64_t bases[] = {2ULL, 325ULL, 9375ULL, 28178ULL,
                                     450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;

        /* Compute x = a^d mod n */
        __uint128_t x = 1;
        __uint128_t base = a % n;
        uint64_t exp = d;
        while (exp > 0) {
            if (exp & 1ULL) x = (x * base) % n;
            base = (base * base) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n - 1) continue;

        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* ----  dynamic storage for the sequence  ---- */
static seq_t *terms      = NULL;   /* cached terms */
static size_t terms_cap   = 0;      /* allocated capacity */
static idx_t  terms_len   = 0;      /* number of stored terms */

static uint64_t next_q = 2;         /* next candidate value for q */

/* Ensure the array has room for `needed` elements */
static void ensure_terms_cap(size_t needed)
{
    if (terms_cap >= needed) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    terms_cap = new_cap;
}

/* Append a new term to the cache */
static void add_term(seq_t value)
{
    ensure_terms_cap(terms_len + 1);
    terms[terms_len++] = value;
}

/* ----  main sequence function  ---- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined behaviour allowed */

    /* Generate terms until we have at least n of them */
    while (terms_len < n) {
        /* Find the next prime q = (p-1)/128 */
        while (!is_prime_uint64(next_q)) {
            ++next_q;
        }

        uint64_t q = next_q;
        uint64_t p = 128ULL * q + 1ULL;          /* candidate prime p */

        /* Test p and 128p+1 for primality */
        if (is_prime_uint64(p) && is_prime_uint64(128ULL * p + 1ULL)) {
            add_term((seq_t)p);
        }

        ++next_q;   /* advance to next integer for the next iteration */
    }

    return terms[n - 1];
}
