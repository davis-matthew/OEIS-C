/*
 * A073885  a(1)=1, a(n) = smallest positive integer not included earlier such
 *          that the n-th partial sum is a prime iff n is a prime.
 * https://oeis.org/A073885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest unused positive integer such that the n‑th
 *            partial sum is prime iff n is prime. Computation is incremental:
 *            to get a(n) we try all integers from 1 upward, skip those
 *            already used, and accept the first that satisfies the primality
 *            condition on the partial sum
 * Range:     u8 n <= 22, u16 n <= 1702, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   prime_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/prime_idx.h"

/* --- Prime test for seq_t (deterministic Miller–Rabin for 64‑bit) --- */
static inline seq_t mod_mul(seq_t a, seq_t b, seq_t mod)
{
    return (seq_t)(((__uint128_t)a * b) % mod);
}

static inline seq_t mod_pow(seq_t a, seq_t e, seq_t mod)
{
    seq_t r = 1;
    while (e)
    {
        if (e & 1) r = mod_mul(r, a, mod);
        a   = mod_mul(a, a, mod);
        e >>= 1;
    }
    return r;
}

static bool is_prime_seq_t(seq_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;

    seq_t d = n - 1;
    int s  = 0;
    while ((d & 1ULL) == 0)
    {
        d >>= 1;
        s++;
    }
    static const seq_t bases[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i)
    {
        seq_t a = bases[i] % n;
        if (a == 0) continue;
        seq_t x = mod_pow(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool comp = true;
        for (int r = 1; r < s; ++r)
        {
            x = mod_mul(x, x, n);
            if (x == n-1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

/* --- Dynamic array helpers (C++ compatible via explicit casts) --- */
static bool *used      = NULL;   /* used[i] is true if integer i already used */
static size_t used_cap = 0;

static void ensure_used(idx_t v)
{
    if (used_cap > v) return;
    size_t new_cap = used_cap ? used_cap << 1 : 16;
    while (new_cap <= (size_t)v) new_cap <<= 1;
    bool *tmp = (bool *)realloc(used, new_cap * sizeof(bool));
    if (!tmp) abort();
    memset(tmp + used_cap, 0, (new_cap - used_cap) * sizeof(bool));
    used = tmp;
    used_cap = new_cap;
}

static seq_t *a_cache   = NULL;   /* a_cache[i] = a(i+1), stored 0‑based */
static seq_t *sum_cache = NULL;   /* sum_cache[i] = sum of first i+1 terms */
static size_t cache_len = 0;

static void ensure_cache(idx_t n)
{
    if (cache_len >= (size_t)n) return;
    size_t new_len = cache_len ? cache_len << 1 : 16;
    while (new_len < (size_t)n) new_len <<= 1;
    seq_t *tmpa = (seq_t *)realloc(a_cache,   new_len * sizeof(seq_t));
    if (!tmpa) abort();
    seq_t *tmpb = (seq_t *)realloc(sum_cache, new_len * sizeof(seq_t));
    if (!tmpb) abort();
    a_cache   = tmpa;
    sum_cache = tmpb;
    /* zero new part of sum_cache */
    memset(sum_cache + cache_len, 0, (new_len - cache_len) * sizeof(seq_t));
    cache_len = new_len;
}

/* --- Main sequence function --- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* undefined for n<1, harmless */

    /* static state between calls */
    static idx_t next = 1;                   /* current candidate */
    static idx_t cur  = 0;                   /* how many terms already known */

    if (cur < n)
    {
        ensure_cache(n);

        for (; cur < n; ++cur)
        {
            bool needPrime = prime_idx((idx_t)(cur + 1));
            /* previous partial sum */
            seq_t prevSum = (cur == 0) ? 0 : sum_cache[cur-1];

            /* search for the smallest unused candidate satisfying condition */
            for (idx_t cand = 1; ; ++cand)
            {
                ensure_used(cand);
                if (used[cand]) continue;

                seq_t newSum = prevSum + (seq_t)cand;
                bool newPrime = is_prime_seq_t(newSum);
                if (newPrime == needPrime)
                {
                    a_cache[cur]   = (seq_t)cand;
                    sum_cache[cur] = newSum;
                    used[cand] = true;
                    break;
                }
            }
        }
    }
    return a_cache[n - 1];
}
