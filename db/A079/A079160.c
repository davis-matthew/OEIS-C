/*
 * A079160  Satisfies A(x)^2 = (1 + Sum_{n>=1} A079159(n)*p_n*x^n) =
 *          A079161(x), where p_n is the n-th prime.
 * https://oeis.org/A079160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1. For n ≥ 1 let pₙ be the n‑th prime (p₁ = 2). Then a(n)
 *            is the smallest positive integer such that Σ_{i=0}^{n}
 *            a(i)·a(n−i) ≡ 0 (mod pₙ). The function below computes a(n) for
 *            arbitrary n, memoising previously computed values and generating
 *            primes on demand
 * Range:     u8 n <= 54, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static seq_t *a_cache   = NULL;   /* memoised a-values          */
static size_t a_len      = 0;      /* number of entries stored   */
static size_t a_cap      = 0;      /* allocated capacity         */

static uint64_t *prime_arr = NULL;/* first primes generated      */
static size_t  prime_len   = 0;    /* number of primes stored    */
static size_t  prime_cap   = 0;    /* allocated prime capacity   */

static void generate_primes_upto(idx_t need)
{
    /* extend prime_arr until it holds 'need' primes          */
    while (prime_len < (size_t)need)
    {
        uint64_t cand;
        if (prime_len == 0)
            cand = 2;
        else
            cand = prime_arr[prime_len - 1] + 1;

        /* find next prime by trial division                 */
        while (true)
        {
            if (cand > 2 && (cand & 1ULL) == 0) { cand++; continue; }

            bool ok = true;
            for (size_t j = 0; j < prime_len; ++j)
            {
                uint64_t p = prime_arr[j];
                if (p * p > cand) break;
                if (cand % p == 0) { ok = false; break; }
            }
            if (ok) break;
            cand++;
        }

        /* grow array if needed                                   */
        if (prime_len == prime_cap)
        {
            size_t new_cap = (prime_cap == 0) ? 16 : prime_cap * 2;
            uint64_t *tmp = (uint64_t *)realloc(prime_arr,
                                                 new_cap * sizeof(uint64_t));
            prime_arr = tmp;
            prime_cap = new_cap;
        }
        prime_arr[prime_len++] = cand;
    }
}

static void ensure_a_cache(idx_t n)
{
    /* grow a_cache until it can hold values up to n           */
    if (a_cap <= (size_t)n)
    {
        size_t new_cap = (a_cap == 0) ? 16 : a_cap * 2;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        a_cache = tmp;
        a_cap   = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* fast return for already computed values */
    if (n < a_len)
        return a_cache[(size_t)n];

    /* make sure cache and prime array are ready        */
    ensure_a_cache(n);
    generate_primes_upto(n);

    /* compute missing a-values                          */
    for (size_t k = a_len; k <= (size_t)n; ++k)
    {
        if (k == 0)
        {
            a_cache[0] = (seq_t)1;
        }
        else if (k == 1)
        {
            /* prime p₁ = 2, a(1) = 1 by definition             */
            a_cache[1] = (seq_t)1;
        }
        else
        {
            uint64_t p = prime_arr[k - 1];      /* pₖ   */
            uint64_t sum_mod = 0;              /* Σ aᵢ·aₖ₋ᵢ  (mod p) */

            for (size_t i = 1; i < k; ++i)
            {
                /* reduce each factor modulo p before multiplying */
                uint64_t ai = (uint64_t)(a_cache[i] % (seq_t)p);
                uint64_t aj = (uint64_t)(a_cache[k - i] % (seq_t)p);
                uint64_t prod = (ai * aj) % p;
                sum_mod = (sum_mod + prod) % p;
            }

            uint64_t rhs   = (p + p - sum_mod) % p;   /* -Σ (mod p)  */
            uint64_t inv2  = (p + 1) / 2;             /* inverse of 2 mod p (p odd) */
            uint64_t a_mod = (rhs * inv2) % p;

            if (a_mod == 0)
                a_cache[k] = (seq_t)p;   /* minimal positive solution      */
            else
                a_cache[k] = (seq_t)a_mod;
        }
    }
    a_len = (size_t)n + 1;
    return a_cache[(size_t)n];
}
