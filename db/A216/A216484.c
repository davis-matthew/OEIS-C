/*
 * A216484  Primes p such that x^36 = 2 has no solution mod p.
 * https://oeis.org/A216484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p for which x^36 ≡ 2 (mod p) has NO
 *            solution
 * Range:     u8 n <= 41, u16 n <= 4981, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (i.e. 2 is not a 36‑th power modulo p) */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* ---------- helper: 128‑bit modular multiplication (works for 64‑bit mods) ---------- */
static inline uint64_t mul_mod_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
#if defined(__SIZEOF_INT128__)
    return (uint64_t)((__uint128_t)a * b % mod);
#else
    /* Fallback for compilers without __uint128_t */
    uint64_t result = 0;
    while (b)
    {
        if (b & 1ULL)
        {
            result = result + a;
            if (result >= mod) result -= mod;
        }
        a <<= 1;
        if (a >= mod) a -= mod;
        b >>= 1;
    }
    return result;
#endif
}

/* ---------- binary modular exponentiation ---------- */
static uint64_t powmod64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp)
    {
        if (exp & 1ULL)
            result = mul_mod_uint64(result, b, mod);
        b = mul_mod_uint64(b, b, mod);
        exp >>= 1ULL;
    }
    return result;
}

/* ---------- GCD for uint64_t ---------- */
static uint64_t gcd64(uint64_t a, uint64_t b)
{
    while (b != 0)
    {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* ---------- prime generation (trial division) ---------- */
static uint64_t *prime_list = NULL;
static size_t    prime_cnt  = 0;
static size_t    prime_cap  = 0;
static uint64_t  next_candidate = 3;

/* Ensure enough room in the prime list */
static void ensure_prime_cap(size_t need)
{
    if (prime_cap < need)
    {
        size_t new_cap = prime_cap ? prime_cap * 2 : 16;
        while (new_cap < need) new_cap <<= 1;
        prime_list = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
        prime_cap = new_cap;
    }
}

/* Return the next prime number (unordered, not cached beyond prime_list) */
static uint64_t next_prime(void)
{
    if (prime_cnt == 0)
    {
        ensure_prime_cap(1);
        prime_list[0] = 2ULL;
        prime_cnt = 1;
        return 2ULL;
    }

    while (1)
    {
        uint64_t cand = next_candidate;
        bool isprime = true;
        uint64_t lim = (uint64_t)(sqrt((double)cand)) + 1;
        for (size_t i = 0; i < prime_cnt && prime_list[i] <= lim; ++i)
        {
            if (cand % prime_list[i] == 0)
            {
                isprime = false;
                break;
            }
        }
        if (isprime)
        {
            ensure_prime_cap(prime_cnt + 1);
            prime_list[prime_cnt++] = cand;
            next_candidate += 2ULL;               /* skip even numbers */
            return cand;
        }
        next_candidate += 2ULL;
    }
}

/* ---------- sequence storage ---------- */
static seq_t *terms = NULL;
static size_t  term_cnt = 0;
static size_t  term_cap = 0;

/* Ensure enough room in the terms array */
static void ensure_term_cap(size_t need)
{
    if (term_cap < need)
    {
        size_t new_cap = term_cap ? term_cap * 2 : 16;
        while (new_cap < need) new_cap <<= 1;
        terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        term_cap = new_cap;
    }
}

/* ---------- the requested OEIS function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1; behaviour for n < 1 is undefined */

    /* Generate terms until we have at least n of them */
    while (term_cnt < (size_t)n)
    {
        uint64_t p = next_prime();
        if (p < 3) continue;            /* sequence starts from prime 3 */

        uint64_t g    = gcd64(36ULL, p - 1ULL);
        uint64_t exp  = (p - 1ULL) / g;
        uint64_t pow  = powmod64(2ULL, exp, p);

        if (pow != 1ULL)                /* 2 is NOT a 36th power modulo p */
        {
            ensure_term_cap(term_cnt + 1);
            terms[term_cnt++] = (seq_t)p;
        }
    }

    return terms[(size_t)(n - 1)];
}
