/*
 * A088708  Numbers m which are a product of two primes j and k such that
 *          m+j-k and m-j+k are both primes.
 * https://oeis.org/A088708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A088708 is defined by a search: the n‑th term is the n‑th *
 *            integer m that is a product of two distinct primes p, q such
 *            that * m+p−q and m−p+q are both prime
 * Range:     u8 n <= 11, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   mod_mul_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mod_mul_u64.h"

/* ----- modular multiplication and exponentiation for 64‑bit ---- */

static uint64_t mod_pow_u64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    while (exp)
    {
        if (exp & 1)
            result = mod_mul_u64(result, base, mod);
        base = mod_mul_u64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* ----- deterministic Miller–Rabin pruning ----- */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] =
        {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i)
    {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    static const uint64_t witnesses[] =
        {2ULL,325ULL,9375ULL,28178ULL,450775ULL,9780504ULL,1795265022ULL};
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    for (size_t i = 0; i < sizeof(witnesses)/sizeof(witnesses[0]); ++i)
    {
        uint64_t a = witnesses[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow_u64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (int r = 1; r < s; ++r)
        {
            x = mod_mul_u64(x, x, n);
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* ----- small prime sieve to 1 000 000 ----- */
static uint32_t *prime_list   = NULL;
static size_t    prime_count  = 0;
static bool      primes_init  = false;

static void initialise_primes(void)
{
    const uint32_t N = 1000000;
    bool *is_composite = (bool*)calloc(N + 1, sizeof(bool));
    for (uint32_t i = 2; (uint64_t)i * i <= N; ++i)
        if (!is_composite[i])
            for (uint32_t j = i * i; j <= N; j += i)
                is_composite[j] = true;
    prime_count = 0;
    for (uint32_t i = 2; i <= N; ++i)
        if (!is_composite[i]) ++prime_count;
    prime_list = (uint32_t*)malloc(prime_count * sizeof(uint32_t));
    size_t idx = 0;
    for (uint32_t i = 2; i <= N; ++i)
        if (!is_composite[i]) prime_list[idx++] = i;
    free(is_composite);
    primes_init = true;
}

/* ----- smallest prime factor via trial division ----- */
static uint64_t smallest_factor(uint64_t n)
{
    if (n % 2 == 0) return 2;
    for (size_t i = 0; i < prime_count; ++i)
    {
        uint32_t p = prime_list[i];
        uint64_t pp = (uint64_t)p * p;
        if (pp > n) break;
        if (n % p == 0) return p;
    }
    return n; /* n is prime */
}

/* ----- sequence cache ----- */
static seq_t *terms      = NULL;
static size_t terms_len  = 0;
static size_t terms_cap  = 0;
static uint64_t next_candidate = 2;   /* start searching from 2 */

/* ----- main sequence function ----- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined below offset */
    if (!primes_init) initialise_primes();

    /* grow cache if needed */
    if (terms_len < (size_t)n)
    {
        size_t needed = (size_t)n;
        if (terms_cap < needed)
        {
            size_t new_cap = terms_cap ? terms_cap << 1 : 16;
            while (new_cap < needed) new_cap <<= 1;
            seq_t *new_terms = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
            if (!new_terms) abort();
            terms = new_terms;
            terms_cap = new_cap;
        }

        /* find further terms */
        while (terms_len < (size_t)n)
        {
            uint64_t m = next_candidate++;
            uint64_t p = smallest_factor(m);
            if (p == m) continue;             /* m is prime */
            uint64_t q = m / p;
            if (q == p) continue;             /* equal primes */
            if (!is_prime_u64(q)) continue;    /* q not prime */

            int64_t diff  = (int64_t)p - (int64_t)q;
            int64_t a_val = (int64_t)m + diff;
            int64_t b_val = (int64_t)m - diff;
            if (!is_prime_u64((uint64_t)a_val)) continue;
            if (!is_prime_u64((uint64_t)b_val)) continue;

            terms[terms_len++] = (seq_t)m;
        }
    }
    return terms[n - 1];
}
