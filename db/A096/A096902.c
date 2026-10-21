/*
 * A096902  Numbers n such that 3*prime(n) + 2*prime(n+1) is prime.
 * https://oeis.org/A096902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th integer i such that 3*prime(i)+2*prime(i+1) is
 *            also prime
 * Range:     u8 n <= 96, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* --------- support for 64‑bit modular arithmetic ---------- */
static uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

static uint64_t modpow(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1 % mod;
    uint64_t base = a % mod;
    while (e) {
        if (e & 1) r = mul_mod(r, base, mod);
        base = mul_mod(base, base, mod);
        e >>= 1;
    }
    return r;
}

/* --------- deterministic Miller–Rabin for 64‑bit numbers ---------- */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    size_t i;
    for (i = 0; i < sizeof(small)/sizeof(small[0]); ++i)
        if (n == small[i]) return true;
    for (i = 0; i < sizeof(small)/sizeof(small[0]); ++i)
        if (n % small[i] == 0) return false;

    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    /* deterministic bases for 64‑bit integers */
    static const uint64_t bases[] = {2,3,5,7,11,13,17};
    for (i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) return true;
        uint64_t x = modpow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

/* --------- main sequence function --------------------------------- */
seq_t a(idx_t n)
{
    /* static tables */
    static uint32_t *primes = NULL;
    static size_t prime_cnt = 0, prime_cap = 0;
    static seq_t *terms  = NULL;
    static size_t term_cnt = 0, term_cap = 0;
    static uint32_t next_candidate = 2;
    static bool initialized = false;
    if (!initialized) {
        prime_cap = 64;
        primes = (uint32_t*)malloc(prime_cap * sizeof(uint32_t));
        term_cap  = 64;
        terms = (seq_t*)malloc(term_cap * sizeof(seq_t));
        initialized = true;
    }

    /* generate primes and collect sequence terms until we have n of them */
    while (term_cnt < n) {
        /* find next prime */
        uint32_t p;
        for (;;) {
            p = next_candidate++;
            if (is_prime_u64(p)) {
                if (prime_cnt == prime_cap) {
                    prime_cap <<= 1;
                    primes = (uint32_t*)realloc(primes, prime_cap * sizeof(uint32_t));
                }
                primes[prime_cnt++] = p;
                break;
            }
        }

        /* when we have at least two primes, test the condition for
           the new index i = prime_cnt - 1  (since primes are 1‑based) */
        if (prime_cnt >= 2) {
            uint64_t expr = 3ULL * primes[prime_cnt-2] + 2ULL * primes[prime_cnt-1];
            if (is_prime_u64(expr)) {
                if (term_cnt == term_cap) {
                    term_cap <<= 1;
                    terms = (seq_t*)realloc(terms, term_cap * sizeof(seq_t));
                }
                terms[term_cnt++] = (seq_t)(prime_cnt - 1);
            }
        }
    }
    return terms[n-1];
}
