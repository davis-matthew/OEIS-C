/*
 * A019405  Primes with primitive root 83.
 * https://oeis.org/A019405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n): the n-th prime p for which 83 is a primitive root modulo p.
 * The sequence starts 2,3,5,11,23,31,59,89,97,101,127,131,137,149,157,167,181,191,...
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- 64‑bit modular exponentiation ---------- */
static uint64_t modpow64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1;
    while (e)
    {
        if (e & 1) r = (uint64_t)((__int128)r * a % mod);
        a = (uint64_t)((__int128)a * a % mod);
        e >>= 1;
    }
    return r;
}

/* ---------- Prime generation (linear sieve) ---------- */
#define SIEVE_LIMIT 2000000U               /* enough for n up to ~10 000 */

static idx_t *prime_list   = NULL;
static size_t prime_cnt     = 0;
static size_t prime_cap     = 0;
static idx_t *spf_array     = NULL;
static bool   tables_built   = false;

/* Build primes up to SIEVE_LIMIT and the smallest‑prime‑factor table. */
static void build_tables(void)
{
    if (tables_built) return;

    /* allocate arrays */
    prime_cap = 200000U;                            /* about 148 000 primes below 2 M */
    prime_list = (idx_t *)malloc(prime_cap * sizeof(idx_t));
    if (!prime_list) exit(1);

    spf_array = (idx_t *)calloc(SIEVE_LIMIT + 1, sizeof(idx_t));
    if (!spf_array) exit(1);

    /* linear sieve */
    idx_t *small_primes = (idx_t *)malloc(prime_cap * sizeof(idx_t));
    if (!small_primes) exit(1);
    size_t small_cnt = 0;

    for (idx_t i = 2; i <= SIEVE_LIMIT; ++i)
    {
        if (spf_array[i] == 0)           /* i is prime */
        {
            spf_array[i] = i;
            if (prime_cnt < prime_cap)
                prime_list[prime_cnt++] = i;
            else            /* grow array if necessary */
            {
                prime_cap <<= 1;
                prime_list = (idx_t *)realloc(prime_list,
                                              prime_cap * sizeof(idx_t));
                if (!prime_list) exit(1);
                prime_list[prime_cnt++] = i;
            }
            small_primes[small_cnt++] = i;
        }

        for (size_t j = 0; j < small_cnt; ++j)
        {
            idx_t p = small_primes[j];
            idx_t ip = i * p;
            if (ip > SIEVE_LIMIT) break;
            spf_array[ip] = p;
            if (p == spf_array[i]) break;
        }
    }
    free(small_primes);
    tables_built = true;
}

/* ---------- Primitive‑root test for 83 modulo a prime p ---------- */
static bool is_primitive_root_83(idx_t p)
{
    if (p == 83ULL) return false;          /* 83 is not a primitive root of itself */

    idx_t m = p - 1;
    idx_t t = m;
    idx_t last_q = 0;

    /* factor m using spf_array; skip repeated factors */
    while (t > 1)
    {
        idx_t q = spf_array[t];
        if (q == 0) q = t;                /* safety for primes > SIEVE_LIMIT (not needed here) */

        if (q != last_q)
        {
            uint64_t exp = (uint64_t)(m / q);
            uint64_t r   = modpow64(83ULL, exp, (uint64_t)p);
            if (r == 1ULL) return false;  /* not a primitive root */
            last_q = q;
        }
        while (t % q == 0) t /= q;
    }
    return true;
}

/* ---------- Sequence cache ---------- */
static seq_t *seq_cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static size_t cache_iter = 0;              /* index into prime_list */

/* Append a value to the cache, growing the array if necessary. */
static void append_cache(seq_t val)
{
    if (cache_len == cache_cap)
    {
        cache_cap = (cache_cap ? cache_cap * 2 : 16);
        seq_cache = (seq_t *)realloc(seq_cache,
                                     cache_cap * sizeof(seq_t));
        if (!seq_cache) exit(1);
    }
    seq_cache[cache_len++] = val;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* safeguard for indices < 1 */

    build_tables();                 /* ensure primes and spf table exist */

    while (cache_len < n)
    {
        if (cache_iter >= prime_cnt)
            return 0;              /* no more primes within pre‑built range */

        idx_t p = prime_list[cache_iter++];
        if (is_primitive_root_83(p))
            append_cache((seq_t)p);
    }
    return seq_cache[n - 1];
}
