/*
 * A096134  a(1) = 2; for n > 1: a(n) = least multiple m of n such that m is
 *          coprime to n+1 and the absolute difference of a(n) and a(n-1) is a
 *          prime distinct from all earlier such differences of consecutive
 *          terms.
 * https://oeis.org/A096134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least multiple m of n such that * gcd(m, n+1) = 1 and |m
 *            - a(n-1)| is a new prime. * a(1) = 2. For n > 1 the algorithm
 *            searches for the * smallest m satisfying the above conditions.
 *            Each prime * difference is recorded and may not appear again.
 *            The * implementation memoises previously compu...
 * Range:     u8 n <= 22, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/gcd_u64.h"

/* Forward declarations – seq_t and idx_t are defined by the build. */
seq_t a(idx_t n);

/* ----------  Local auxiliary functions   ---------- */

/* Fast modular exponentiation for 64-bit integers. */
static uint64_t modpow64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1;
    while (e)
    {
        if (e & 1ULL)
            r = (unsigned __int128)r * a % mod;
        a = (unsigned __int128)a * a % mod;
        e >>= 1ULL;
    }
    return r;
}

/* Deterministic Miller–Rabin for 64‑bit integers. */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i)
    {
        if (n % small[i] == 0ULL)
            return n == small[i];
    }
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0ULL) { d >>= 1; ++s; }
    static const uint64_t bases[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = modpow64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (int r = 1; r < s; ++r)
        {
            x = (unsigned __int128)x * x % n;
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Euclidean GCD for 64‑bit integers. */

/* Dynamically grow the array that stores the sequence values. */
static seq_t *seq_cache = NULL;
static idx_t seq_cache_len = 0;      /* how many terms are already computed */
static idx_t seq_cache_alloc = 0;    /* allocated size */

/* Ensure that the cache can hold at least idx terms (indexing from 1). */
static void ensure_seq_cache(idx_t idx)
{
    if (seq_cache_alloc < idx + 1)
    {
        idx_t newalloc = seq_cache_alloc == 0 ? 16 : seq_cache_alloc * 2;
        while (newalloc < idx + 1)
            newalloc *= 2;
        seq_t *tmp = (seq_t *)realloc(seq_cache, newalloc * sizeof(seq_t));
        if (!tmp) abort();  /* out‑of‑memory */
        if (tmp != seq_cache) { seq_cache = tmp; }
        memset(seq_cache + seq_cache_alloc, 0, (newalloc - seq_cache_alloc) * sizeof(seq_t));
        seq_cache_alloc = newalloc;
    }
}

/* Dynamically grow the bitset that records used prime differences. */
static unsigned char *prime_used = NULL;  /* 0/1 for each prime */
static size_t prime_used_alloc = 0;       /* allocated size in bytes */

/* Ensure that prime_used contains at least pos entries. */
static void ensure_prime_used(size_t pos)
{
    if (prime_used_alloc <= pos)
    {
        size_t newalloc = prime_used_alloc == 0 ? 16 : prime_used_alloc * 2;
        while (newalloc <= pos)
            newalloc <<= 1;
        unsigned char *tmp = (unsigned char *)realloc(prime_used, newalloc);
        if (!tmp) abort();
        if (tmp != prime_used) { prime_used = tmp; }
        memset(prime_used + prime_used_alloc, 0, (newalloc - prime_used_alloc));
        prime_used_alloc = newalloc;
    }
}

/* ----------  Main function   ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined; return 0 for safety */

    /* Grow cache if needed. */
    ensure_seq_cache(n);

    /* Compute terms up to n if they have not been computed yet. */
    if (seq_cache_len < n)
    {
        /* Term a(1). */
        if (seq_cache_len == 0)
        {
            seq_cache[1] = (seq_t)2;
            seq_cache_len = 1;
        }

        for (idx_t cur_n = seq_cache_len + 1; cur_n <= n; ++cur_n)
        {
            seq_t prev = seq_cache[cur_n - 1];
            idx_t n_plus1 = cur_n + 1;      /* n+1 for this step        */
            uint64_t n_val = (uint64_t)cur_n;
            uint64_t n_plus1_val = (uint64_t)n_plus1;

            /* Search candidates: multiples of cur_n. */
            uint64_t k = 1;  /* multiplier */
            while (1)
            {
                seq_t m = (seq_t)(n_val * k);   /* current candidate m */
                /* Compute |m - prev| as unsigned difference. */
                seq_t diff = (m > prev) ? (m - prev) : (prev - m);
                uint64_t diff64 = (uint64_t)diff;   /* fits in 64‑bit for used values */

                if (diff64 > 1 && is_prime_u64(diff64))
                {
                    /* Make sure our used‑prime bitset is large enough. */
                    ensure_prime_used((size_t)(diff64 + 1));
                    if (!prime_used[diff64])          /* distinctness */
                    {
                        /* GCD condition: m coprime to n+1. */
                        if (gcd_u64((uint64_t)m, n_plus1_val) == 1ULL)
                        {
                            seq_cache[cur_n] = m;
                            prime_used[diff64] = 1;  /* mark as used */
                            break;
                        }
                    }
                }
                ++k;
                /* Safety: if k becomes too large we still continue;
                   the existence of a valid m is guaranteed by the
                   definition of the sequence. */
            }
        }
        seq_cache_len = n;
    }
    return seq_cache[n];
}
