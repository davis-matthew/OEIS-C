/*
 * A107288  Primes whose digit sum is a square.
 * https://oeis.org/A107288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 981, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mul_mod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A107288: the nth prime whose digit sum is a perfect square. 
   The sequence is obtained by a simple incremental search:  
   candidates are tested for primality (deterministic Miller–Rabin for 64‑bit
   integers) and for the property that the sum of their decimal digits is a
   square.  Results are memoised in a static array so that any order of
   calls is handled efficiently. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mul_mod64.h"

/* ---------- Modular arithmetic helpers ---------- */

static uint64_t pow_mod64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1;
    while (exp)
    {
        if (exp & 1)
            res = mul_mod64(res, base, mod);
        base = mul_mod64(base, base, mod);
        exp >>= 1;
    }
    return res;
}

/* ---------- Deterministic Miller–Rabin for 64‑bit integers ---------- */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)
        return false;
    /* small‑prime trial division */
    static const uint64_t smalls[] = {
        2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL
    };
    for (size_t i = 0; i < sizeof(smalls)/sizeof(smalls[0]); ++i)
    {
        uint64_t p = smalls[i];
        if (n % p == 0)
            return n == p;
    }
    /* Miller‑Rabin with bases that are deterministic for 64‑bit
       (see e.g. https://en.wikipedia.org/wiki/Miller%E2%80%93Rabin_primality_test ) */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0)
    {
        d >>= 1;
        ++s;
    }
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t idx = 0; idx < sizeof(bases)/sizeof(bases[0]); ++idx)
    {
        uint64_t a = bases[idx] % n;
        if (a == 0)
            continue;
        uint64_t x = pow_mod64(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        bool cont = false;
        for (int r = 1; r < s; ++r)
        {
            x = mul_mod64(x, x, n);
            if (x == n - 1)
            {
                cont = true;
                break;
            }
        }
        if (!cont)
            return false;
    }
    return true;
}

/* ---------- Elementary helpers ---------- */
static uint64_t digit_sum_uint64(uint64_t x)
{
    uint64_t s = 0;
    while (x)
    {
        s += x % 10;
        x /= 10;
    }
    return s;
}

static uint64_t isqrt_uint64(uint64_t n)
{
    uint64_t lo = 0, hi = 3037000499ULL; /* floor(sqrt(2^64-1)) */
    while (lo <= hi)
    {
        uint64_t mid = (lo + hi) >> 1;
        uint64_t sq = (unsigned __int128)mid * mid;
        if (sq == n)
            return mid;
        if (sq < n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

static bool is_square_uint64(uint64_t n)
{
    uint64_t r = isqrt_uint64(n);
    return r * r == n;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* safety; not used by OEIS */

    /* Memoisation structures */
    static seq_t *terms = NULL;
    static idx_t terms_cnt = 0;
    static idx_t terms_cap = 0;
    static uint64_t last_checked = 1;   /* next candidate to examine */

    /* initialise on first call */
    if (terms == NULL)
    {
        terms_cap = 128;
        terms = (seq_t*)malloc((size_t)terms_cap * sizeof(seq_t));
    }
    if (terms == NULL)   /* allocation failed – just return 0 */
        return (seq_t)0;

    /* generate terms until we have at least n of them */
    while (terms_cnt < n)
    {
        uint64_t candidate = ++last_checked;
        if (!is_prime_uint64(candidate))
            continue;
        uint64_t ds = digit_sum_uint64(candidate);
        if (!is_square_uint64(ds))
            continue;

        /* store the found term */
        if (terms_cnt >= terms_cap)
        {
            terms_cap <<= 1;                    /* double capacity */
            seq_t *tmp = (seq_t*)realloc(terms,
                                         (size_t)terms_cap * sizeof(seq_t));
            if (tmp == NULL)
                return (seq_t)0;                /* realloc failure */
            terms = tmp;
        }
        terms[terms_cnt++] = (seq_t)candidate;
    }

    return terms[n - 1];
}
