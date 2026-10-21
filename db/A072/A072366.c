/*
 * A072366  Numbers x such that x + reverse of x is a prime.
 * https://oeis.org/A072366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 5312, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers x such that x + reverse(x) is a prime. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* ----- primes helpers --------------------------------------------------- */

/* 128‑bit multiplication modulo mod, using the compiler's 128‑bit type. */
static uint64_t mul_mod_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)(((__uint128_t)a * b) % mod);
}

/* Modular exponentiation (a^e mod mod). */
static uint64_t pow_mod_uint64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t res = 1;
    a %= mod;
    while (e) {
        if (e & 1) res = mul_mod_uint64(res, a, mod);
        a = mul_mod_uint64(a, a, mod);
        e >>= 1;
    }
    return res;
}

/* Deterministic Miller–Rabin for 64‑bit unsigned integers. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    /* small primes first */
    static const uint64_t small_primes[] = {
        2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    /* write n-1 = d * 2^s */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    /* bases sufficient for deterministic test < 2^64 */
    static const uint64_t bases[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod_uint64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (int r = 1; r < s; ++r) {
            x = mul_mod_uint64(x, x, n);
            if (x == n - 1) {
                cont = true;
                break;
            }
        }
        if (!cont) return false;
    }
    return true;
}

/* ----- decimal reverse --------------------------------------------------- */

static uint64_t reverse_uint64(uint64_t x)
{
    uint64_t rev = 0;
    while (x) {
        rev = rev * 10 + (x % 10);
        x /= 10;
    }
    return rev;
}

/* ----- memoised term generation ------------------------------------------ */

static seq_t *terms = NULL;         /* dynamic array of found terms      */
static size_t terms_cap = 0;        /* capacity of array                 */
static size_t terms_len = 0;        /* number of elements in array       */
static uint64_t next_candidate = 1; /* smallest candidate not yet tested */

static void push_term(seq_t val)
{
    if (terms_len == terms_cap) {
        terms_cap = terms_cap ? terms_cap * 2 : 16;
        terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
    }
    terms[terms_len++] = val;
}

/* ----- the required function --------------------------------------------- */

seq_t a(idx_t n)
{
    /* Generate terms until we have at least n of them. */
    while (terms_len < (size_t)n) {
        uint64_t rev = reverse_uint64(next_candidate);
        uint64_t sum = next_candidate + rev;
        if (is_prime_uint64(sum)) {
            push_term((seq_t)next_candidate);
        }
        ++next_candidate;
    }
    return terms[n - 1];
}
