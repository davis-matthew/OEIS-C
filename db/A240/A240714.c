/*
 * A240714  Even numbers whose unordered two primes decomposition set does not
 *          contain two groups of n = p1+p2 = p3+p4 such that |p1-p3| = 6 or
 *          12.
 * https://oeis.org/A240714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes OEIS A240714 by brute‑force search.
   For each even candidate m, it collects the smaller primes p of all
   unordered Goldbach partitions m = p + q.
   If any two such primes differ by 6 or 12, m is rejected.
   Accepted m are stored in a static cache so that repeated calls are
   fast and independent of call order. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mod_mul.h"

/* ---------- simple 64‑bit Miller‑Rabin primality test ---------- */

static inline uint64_t mod_pow(uint64_t a, uint64_t d, uint64_t mod)
{
    uint64_t r = 1;
    while (d)
    {
        if (d & 1) r = mod_mul(r, a, mod);
        a = mod_mul(a, a, mod);
        d >>= 1;
    }
    return r;
}

static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i)
        if (n == small[i]) return true;
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i)
        if (n % small[i] == 0) return false;

    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool comp = true;
        for (uint64_t r = 1; r < s; ++r)
        {
            x = mod_mul(x, x, n);
            if (x == n-1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

/* ---------- simple sieve for primes up to an odd limit ---------- */
static uint64_t *prime_list = NULL;   /* array of primes */
static size_t      prime_cnt  = 0;
static uint64_t    prime_lim  = 0;    /* largest index for which primes are known */

static void sieve_up_to(uint64_t limit)
{
    if (limit <= prime_lim) return;

    size_t sz = limit + 1;
    bool *comp = (bool*)calloc(sz, sizeof(bool));
    if (!comp) abort();

    for (uint64_t i = 2; i * i <= limit; ++i)
        if (!comp[i])
            for (uint64_t j = i * i; j <= limit; j += i)
                comp[j] = true;

    size_t cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!comp[i]) ++cnt;

    uint64_t *new_primes = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!new_primes) abort();
    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!comp[i]) new_primes[idx++] = i;

    free(comp);
    free(prime_list);
    prime_list = new_primes;
    prime_cnt  = cnt;
    prime_lim  = limit;
}

/* ---------- cache for computed terms ---------- */
static seq_t *term_cache = NULL;
static size_t term_len   = 0;
static size_t term_cap   = 0;
static idx_t  next_candidate = 2;   /* next even number to test */

/* ---------- append a new term to the cache ---------- */
static void append_term(seq_t val)
{
    if (term_len == term_cap)
    {
        term_cap = (term_cap == 0) ? 8 : term_cap * 2;
        term_cache = (seq_t*)realloc(term_cache, term_cap * sizeof(seq_t));
        if (!term_cache) abort();
    }
    term_cache[term_len++] = val;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* guard for underflow */

    /* keep searching until we have at least n terms */
    while (term_len < n)
    {
        uint64_t m = (uint64_t)next_candidate;   /* even candidate */

        /* hold smaller primes of valid partitions */
        uint64_t primes_of_m[512];
        size_t   pcnt = 0;
        bool     ok   = true;

        /* ensure we have primes up to m/2 */
        uint64_t half = m / 2;
        sieve_up_to(half);

        /* examine all prime partitions */
        for (size_t i = 0; i < prime_cnt && prime_list[i] <= half; ++i)
        {
            uint64_t p = prime_list[i];
            uint64_t q = m - p;
            if (q < p) continue;          /* keep unordered pairs */
            if (!is_prime_u64(q)) continue;

            /* check against existing smaller primes */
            for (size_t j = 0; j < pcnt; ++j)
            {
                uint64_t diff = (p > primes_of_m[j]) ? p - primes_of_m[j]
                                                      : primes_of_m[j] - p;
                if (diff == 6 || diff == 12)
                {
                    ok = false;
                    break;
                }
            }
            if (!ok) break;

            /* store this smaller prime */
            if (pcnt < sizeof(primes_of_m)/sizeof(primes_of_m[0]))
                primes_of_m[pcnt++] = p;
        }

        if (ok) append_term((seq_t)m);

        next_candidate += 2;   /* next even number */
    }

    return term_cache[n - 1];
}
