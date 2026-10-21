/*
 * A231178  Let A={1,2,5,6,8,11,13,...} be the sequence of numbers k>=1 such
 *          that k+4 is evil (A001969), and let B be the complement of A. The
 *          sequence lists numbers for which the number of A-divisors equals
 *          number of B-divisors.
 * https://oeis.org/A231178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th perfect square whose proper divisors split
 *            evenly into A-divisors (numbers k with popcount(k+4) even) and
 *            B-divisors
 * Range:     u8 n <= 3, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef uint64_t u64;

/* ------------------------------------------------------------
   Prime sieve – up to a fixed limit sufficient for the search
   ------------------------------------------------------------ */
static uint32_t *prime_tbl = NULL;
static size_t prime_cnt = 0;

static void init_primes(void)
{
    if (prime_tbl != NULL)
        return;

    const size_t LIM = 200000;           /* more than enough for i up to ~10⁶ */
    bool *sieve = (bool*)malloc(LIM + 1);
    memset(sieve, 1, LIM + 1);
    sieve[0] = sieve[1] = false;
    for (size_t p = 2; p * p <= LIM; ++p) {
        if (sieve[p]) {
            for (size_t j = p * p; j <= LIM; j += p)
                sieve[j] = false;
        }
    }
    for (size_t i = 2; i <= LIM; ++i)
        if (sieve[i])
            ++prime_cnt;
    prime_tbl = (uint32_t*)malloc(prime_cnt * sizeof(uint32_t));
    size_t idx = 0;
    for (size_t i = 2; i <= LIM; ++i)
        if (sieve[i])
            prime_tbl[idx++] = (uint32_t)i;
    free(sieve);
}

/* ------------------------------------------------------------
   Trial factorization of a 64‑bit integer
   ------------------------------------------------------------ */
static void factor_u64(u64 n, uint32_t *pf, uint32_t *pe, size_t *pcnt)
{
    size_t cnt = 0;
    for (size_t i = 0; i < prime_cnt &&
                       (u64)prime_tbl[i] * prime_tbl[i] <= n; ++i) {
        u64 p = prime_tbl[i];
        if (n % p == 0) {
            uint32_t e = 0;
            while (n % p == 0) { n /= p; ++e; }
            pf[cnt] = (uint32_t)p;
            pe[cnt] = e;
            ++cnt;
        }
    }
    if (n > 1) {
        pf[cnt]  = (uint32_t)n;
        pe[cnt]  = 1;
        ++cnt;
    }
    *pcnt = cnt;
}

/* ------------------------------------------------------------
   Recursive enumeration of all divisors of s (represented by
   prime factors and exponents), counting how many are in A and B.
   ------------------------------------------------------------ */
static void dfs_div(size_t depth, u64 cur, const uint32_t *pf,
                    const uint32_t *pe, size_t pf_cnt,
                    u64 s, u64 *cntA, u64 *cntB)
{
    if (depth == pf_cnt) {
        if (cur != s) {
            int pc = __builtin_popcountll(cur + 4ULL);
            if ((pc & 1) == 0)
                ++(*cntA);
            else
                ++(*cntB);
        }
        return;
    }
    u64 val = cur;
    for (uint32_t e = 0; e <= pe[depth]; ++e) {
        dfs_div(depth + 1, val, pf, pe, pf_cnt, s, cntA, cntB);
        val *= pf[depth];
    }
}

/* ------------------------------------------------------------
   Cache of discovered terms
   ------------------------------------------------------------ */
static seq_t *cache   = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t  next_i   = 1;   /* next integer to test (its square) */

static void ensure_cache_cap(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* ------------------------------------------------------------
   Main function requested by OEIS entry
   ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    init_primes();

    while (cache_len < (size_t)n) {
        u64 i = (u64)next_i;
        u64 s = i * i;   /* the square to test */

        /* factor i */
        uint32_t pf[64], pe[64];
        size_t pf_cnt;
        factor_u64(i, pf, pe, &pf_cnt);

        /* exponents for s = i² are twice those of i */
        uint32_t pe_s[64];
        for (size_t k = 0; k < pf_cnt; ++k)
            pe_s[k] = pe[k] * 2;

        /* count A- and B-divisors */
        u64 cntA = 0, cntB = 0;
        dfs_div(0, 1, pf, pe_s, pf_cnt, s, &cntA, &cntB);

        if (cntA == cntB) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = (seq_t)s;
        }

        ++next_i;
    }
    return cache[(size_t)n - 1];
}
