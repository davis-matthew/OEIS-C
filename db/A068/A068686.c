/*
 * A068686  Let p = abc..k be a prime in base 10. Define mirror(p) =
 *          abc...k...cba. Sequence gives primes of the form mirror(p) for
 *          some p.
 * https://oeis.org/A068686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 697, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   mul64_mod, pow64_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the n-th prime of the form
 *
 * m = p * 10^(d-1) + (reverse(p) mod 10^(d-1))
 *
 * where p is a prime with d ≥ 2 digits.  The term m itself must be prime.
 *
 * The function builds the sequence by scanning primes p in increasing order
 * and testing the constructed m for primality.  Results are cached so that
 * arbitrary non‑increasing calls to a(idx_t n) are fast.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mul64_mod.h"
#include "../utils/pow64_mod.h"

/* deterministic Miller–Rabin for 64‑bit integers */

static bool isPrime64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof small/sizeof small[0]; ++i) {
        if (n == small[i]) return true;
        if (n % small[i] == 0) return false;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    for (size_t i = 0; i < sizeof small/sizeof small[0]; ++i) {
        uint64_t a = small[i] % n;
        if (a == 0) continue;
        uint64_t x = pow64_mod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul64_mod(x, x, n);
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* power10 table up to 10^18 */
static const uint64_t power10[19] = {
    1ULL,
    10ULL, 100ULL, 1000ULL, 10000ULL, 100000ULL,
    1000000ULL, 10000000ULL, 100000000ULL, 1000000000ULL,
    10000000000ULL, 100000000000ULL, 1000000000000ULL,
    10000000000000ULL, 100000000000000ULL, 1000000000000000ULL,
    10000000000000000ULL, 100000000000000000ULL, 1000000000000000000ULL
};

/* dynamic array for terms */
static seq_t *terms = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;
static void append_term(seq_t v)
{
    if (terms_len + 1 > terms_cap) {
        size_t newcap = terms_cap ? terms_cap * 2 : 8;
        terms = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
        terms_cap = newcap;
    }
    terms[terms_len++] = v;
}

/* dynamic array for primes (uint64_t) */
static uint64_t *primes_u64 = NULL;
static size_t primes_u64_len = 0;
static size_t primes_u64_cap = 0;
static void append_prime_u64(uint64_t v)
{
    if (primes_u64_len + 1 > primes_u64_cap) {
        size_t newcap = primes_u64_cap ? primes_u64_cap * 2 : 16;
        primes_u64 = (uint64_t *)realloc(primes_u64, newcap * sizeof(uint64_t));
        primes_u64_cap = newcap;
    }
    primes_u64[primes_u64_len++] = v;
}

/* prime generator */
static uint64_t prime_candidate = 2;
static size_t next_prime_index = 0;
static uint64_t next_prime(void)
{
    while (1) {
        uint64_t p = prime_candidate;
        /* skip even >2 */
        if (p > 2 && (p & 1) == 0) { prime_candidate = p + 1; continue; }
        /* trial division using primes already found */
        bool ok = true;
        for (size_t i = 0; i < primes_u64_len && primes_u64[i] * primes_u64[i] <= p; ++i) {
            if (p % primes_u64[i] == 0) { ok = false; break; }
        }
        if (ok) {
            append_prime_u64(p);
            /* advance candidate */
            prime_candidate = (p == 2) ? 3 : p + 2;
            return p;
        }
        prime_candidate = (p == 2) ? 3 : p + 2;
    }
}

/* reverse integer */
static uint64_t reverse_uint64(uint64_t n)
{
    uint64_t r = 0;
    while (n) {
        r = r * 10 + (n % 10);
        n /= 10;
    }
    return r;
}

/* compute digits of n (n>=1) */
static int digits_uint64(uint64_t n)
{
    int d = 0;
    while (n) { ++d; n /= 10; }
    return d;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for offset 1, but safe */
    /* ensure we have at least n terms */
    while (terms_len < n) {
        uint64_t p = next_prime();
        if (p < 10) continue; /* skip primes with <2 digits */
        int d = digits_uint64(p);
        uint64_t pow = power10[d-1];
        uint64_t rev = reverse_uint64(p);
        uint64_t rev_no_first = rev % pow;
        uint64_t m = p * pow + rev_no_first;
        if (isPrime64(m))
            append_term((seq_t)m);
    }
    return terms[n-1];
}
