/*
 * A061323  Primes with 10 as smallest positive primitive root.
 * https://oeis.org/A061323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 140, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS A061323 sequence by searching primes and
 * checking that 10 is the smallest positive primitive root. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static inline uint64_t powmod_uint64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1 % mod;
    while (e)
    {
        if (e & 1ULL)
            r = (unsigned __int128)r * a % mod;
        a = (unsigned __int128)a * a % mod;
        e >>= 1;
    }
    return r;
}

static uint32_t isqrt_uint64(uint64_t v)
{
    uint32_t r = 0;
    uint32_t bit = 0x80000000U;   /* 1 << 30 */
    while (bit > v)
        bit >>= 2;
    while (bit)
    {
        if (v >= r + bit)
        {
            v -= r + bit;
            r = (r >> 1) + bit;
        }
        else
            r >>= 1;
        bit >>= 2;
    }
    return r;
}

/* ---------- Prime sieve helpers ----------------------------------- */

static uint32_t *prime_list = NULL;
static size_t    prime_cnt  = 0;
static size_t    prime_cap  = 0;
static uint32_t  last_checked = 1;

static void ensure_primes_upto(uint32_t limit)
{
    if (limit <= last_checked)
        return;

    if (prime_cap < prime_cnt + (limit - last_checked) / 2 + 20)
    {
        size_t new_cap = prime_cnt + (limit - last_checked) / 2 + 20;
#if defined(__cplusplus)
        prime_list = reinterpret_cast<uint32_t *>(
            realloc(prime_list, new_cap * sizeof(uint32_t)));
#else
        prime_list = realloc(prime_list, new_cap * sizeof(uint32_t));
#endif
        if (!prime_list)
            exit(1);
        prime_cap = new_cap;
    }

    for (uint32_t cand = last_checked + 1; cand <= limit; ++cand)
    {
        bool is_p = true;
        uint32_t rc = isqrt_uint64(cand);
        for (size_t i = 0; i < prime_cnt && prime_list[i] <= rc; ++i)
        {
            if (cand % prime_list[i] == 0)
            {
                is_p = false;
                break;
            }
        }
        if (is_p)
            prime_list[prime_cnt++] = cand;
    }
    last_checked = limit;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)
        return false;
    if (n % 2ULL == 0ULL)
        return n == 2ULL;
    uint32_t rc = isqrt_uint64(n);
    ensure_primes_upto(rc);
    for (size_t i = 0; i < prime_cnt && prime_list[i] <= rc; ++i)
    {
        if (n % prime_list[i] == 0ULL)
            return false;
    }
    return true;
}

/* ---------- check that 10 is the smallest primitive root -------- */

static bool ten_is_smallest_primitive_root(uint32_t p)
{
    if (p == 2 || p == 5)
        return false;            /* 10 ≡ 0 (mod p) */

    uint32_t p1 = p - 1;
    uint32_t rc = isqrt_uint64(p1);
    ensure_primes_upto(rc);

    /* factor p-1 */
    uint32_t factors[32];
    size_t nf = 0;
    uint32_t tmp = p1;
    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= tmp; ++i)
    {
        uint32_t q = prime_list[i];
        if (tmp % q == 0)
        {
            factors[nf++] = q;
            while (tmp % q == 0)
                tmp /= q;
        }
    }
    if (tmp > 1)
        factors[nf++] = tmp;

    /* 10 must have order p-1 */
    for (size_t i = 0; i < nf; ++i)
    {
        uint32_t exp = p1 / factors[i];
        if (powmod_uint64(10ULL, exp, p) == 1ULL)
            return false;
    }

    /* no smaller integer 2..9 may be a primitive root */
    for (uint32_t a = 2; a <= 9; ++a)
    {
        bool prim = true;
        for (size_t i = 0; i < nf; ++i)
        {
            uint32_t exp = p1 / factors[i];
            if (powmod_uint64(a, exp, p) == 1ULL)
            {
                prim = false;
                break;
            }
        }
        if (prim)
            return false;
    }
    return true;
}

/* ---------- dynamic storage of the sequence --------------------- */

static seq_t *terms = NULL;
static size_t terms_cap  = 0;
static size_t terms_cnt  = 0;
static uint64_t last_candidate = 1;

static void ensure_n_terms(size_t n)
{
    if (terms_cnt >= n)
        return;

    if (terms_cap < n + 10)
    {
        size_t new_cap = n + 10;
#if defined(__cplusplus)
        terms = reinterpret_cast<seq_t *>(
            realloc(terms, new_cap * sizeof(seq_t)));
#else
        terms = realloc(terms, new_cap * sizeof(seq_t));
#endif
        if (!terms)
            exit(1);
        terms_cap = new_cap;
    }

    uint64_t cand = last_candidate;
    while (terms_cnt < n)
    {
        ++cand;
        if ((cand & 1ULL) == 0ULL)
            continue;                      /* skip even numbers */
        if (!is_prime_uint64(cand))
            continue;                      /* skip composite */

        if (ten_is_smallest_primitive_root((uint32_t)cand))
        {
            terms[terms_cnt++] = (seq_t)cand;
        }
    }
    last_candidate = cand;
}

/* ---------- required function -------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                         /* behaviour below offset irrelevant */
    ensure_n_terms((size_t)n);
    return terms[(size_t)n - 1];
}
