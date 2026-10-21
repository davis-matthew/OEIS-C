/*
 * A225517  Integers m > 1 such that expression m!/(prime(m-1)+1) is not an
 *          integer.
 * https://oeis.org/A225517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 1100, u32 n <= 1100, u64 n <= 1100,
 *            u128 n <= 1100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A225517: integers m > 1 such that m!/(prime(m-1)+1) is not an integer.
   The function builds the sequence incrementally, keeping a vector of primes
   enough for the current index and a vector of sequence values.  For each
   new m it factors prime(m-1)+1 and applies Legendre's formula to determine
   whether the factorial contains the required prime powers.  No memoisation
   other than the stored vectors is used, so calls can arrive in any order. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *prime_vec = NULL;   /* list of primes, 1‑based indexing  */
static size_t prime_cnt = 0;         /* how many primes are stored          */
static size_t prime_cap = 0;         /* allocated capacity                  */
static uint64_t next_prime_candidate = 2;  /* next integer to test for primality */

static uint64_t *seq_vec = NULL;     /* sequence values, 0‑based indexing   */
static size_t seq_cnt = 0;           /* how many values are stored         */
static size_t seq_cap = 0;           /* allocated capacity                  */

static idx_t last_m_checked = 1;     /* last m that has been examined     */

static inline void ensure_primes(size_t needed)
{
    /* Grow prime list until it contains at least `needed` primes. */
    if (prime_cnt >= needed) return;
    for (;;)
    {
        bool is_p = true;
        for (size_t i = 0; i < prime_cnt && prime_vec[i] * prime_vec[i] <= next_prime_candidate; ++i)
        {
            if (next_prime_candidate % prime_vec[i] == 0)
            {
                is_p = false;
                break;
            }
        }
        if (is_p)
        {
            if (prime_cnt == prime_cap)
            {
                size_t new_cap = prime_cap ? prime_cap * 2 : 16;
                prime_vec = (uint64_t *)realloc((void *)prime_vec, new_cap * sizeof(uint64_t));
                prime_cap = new_cap;
            }
            prime_vec[prime_cnt++] = next_prime_candidate;
        }
        ++next_prime_candidate;
        if (prime_cnt >= needed) break;
    }
}

static inline void ensure_seq_cap(size_t need)
{
    if (seq_cap >= need) return;
    while (seq_cap < need)
    {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        seq_vec = (uint64_t *)realloc((void *)seq_vec, new_cap * sizeof(uint64_t));
        seq_cap = new_cap;
    }
}

static inline bool m_is_not_divisible(idx_t m)
{
    /* Determine if m! is NOT divisible by prime(m-1)+1.                 */
    /* prime(m-1) exists because we have at least (m-1) primes stored.  */
    uint64_t p = prime_vec[m - 2];          /* because prime_vec is 0‑based          */
    uint64_t target = p + 1;                /* number we test divisibility for      */

    /* Factor target using primes we already have. */
    uint64_t rem = target;
    struct { uint64_t q; unsigned int exp; } fac[20];
    size_t fac_cnt = 0;
    for (size_t i = 0; i < prime_cnt && prime_vec[i] * prime_vec[i] <= rem; ++i)
    {
        if (rem % prime_vec[i] == 0)
        {
            unsigned int e = 0;
            while (rem % prime_vec[i] == 0)
            {
                rem /= prime_vec[i];
                ++e;
            }
            fac[fac_cnt].q = prime_vec[i];
            fac[fac_cnt].exp = e;
            ++fac_cnt;
        }
    }
    if (rem > 1)
    {
        fac[fac_cnt].q = rem;
        fac[fac_cnt].exp = 1;
        ++fac_cnt;
    }

    /* For each prime factor, compute exponent in m! via Legendre's formula. */
    for (size_t i = 0; i < fac_cnt; ++i)
    {
        uint64_t q = fac[i].q;
        unsigned int need = fac[i].exp;
        unsigned long long have = 0;
        unsigned long long div = m;
        while (div)
        {
            div /= q;
            have += div;
        }
        if (have < need) return true;   /* not divisible */
    }
    return false;  /* divisible */
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* outside the defined range */

    /* Ensure we have enough primes for indices up to m-1 (m will be >= 2). */
    /* During the loop below we extend primes as needed. */

    while (seq_cnt < (size_t)n)
    {
        ++last_m_checked;  /* try next m */
        if (!prime_vec || prime_cnt < (size_t)(last_m_checked - 1))
            ensure_primes((size_t)(last_m_checked - 1));

        if (m_is_not_divisible(last_m_checked))
        {
            ensure_seq_cap(seq_cnt + 1);
            seq_vec[seq_cnt++] = (uint64_t)last_m_checked;
        }
    }
    return (seq_t)seq_vec[n - 1];
}
