/*
 * A067937  Let rep(k) be the k-th repunit number, sequence gives values of k
 *          such that rep(k) == rep(4) (mod k).
 * https://oeis.org/A067937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Compute OEIS A067937: a(n) is the n‑th integer k such that the repunit
   R(k) = (10^k – 1)/9 is congruent to R(4) (=1111) modulo k.
   For each k we test if R(k) ≡ 1111 (mod k) by evaluating T = 10^k (mod 9k)
   and using the identity R(k) = (T – 1)/9.  The result is cached so that
   repeated calls to a() are efficient.
*/

#include <stdlib.h>
#include <stdint.h>

/* auxiliary: modular exponentiation with 128‑bit multiplication */
static uint64_t modpow_uint64(uint64_t base, idx_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b      = base % mod;
    while (exp > 0)
    {
        if (exp & 1U)
            result = (unsigned __int128)result * b % mod;
        b = (unsigned __int128)b * b % mod;
        exp >>= 1U;
    }
    return result;
}

/* compute R(k) mod k.  Uses the formula R(k) = (10^k – 1)/9 (mod k). */
static seq_t rep_mod_k(idx_t k)
{
    /* modulus for the exponentiation */
    uint64_t mod9k = 9ULL * (uint64_t)k;
    uint64_t t = modpow_uint64(10ULL, k, mod9k);   /* 10^k mod 9k */
    /* t is never 0 because 10^k is not divisible by 3, hence not by 9k */
    uint64_t t_minus = t + mod9k - 1;               /* = 10^k – 1 (mod 9k) */
    uint64_t rep_mod = (t_minus / 9ULL) % (uint64_t)k;
    return (seq_t)rep_mod;
}

/* ensures the internal cache can hold at least 'need' elements */
static void ensure_cache(size_t need)
{
    static size_t cap = 0;
    static seq_t *cache = NULL;
    (void)cache;  /* silence unused warning until first access */

    if (cache == NULL && cap == 0)
    {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }
    while (cap < need)
    {
        size_t ncap = cap * 2;
        if (ncap == 0) ncap = 16;
        seq_t *tmp = (seq_t *)realloc(cache, ncap * sizeof(seq_t));
        cache = tmp;
        cap = ncap;
    }
    /* the cache pointer is stored in the static variable below */
    /* but we need to expose it; we use static variables in the outer function */
}

/* main sequence function */
seq_t a(idx_t n)
{
    static seq_t *vals = NULL;
    static size_t   sz   = 0;
    static idx_t    next = 1;   /* next k to test */

    /* lazily allocate cache */
    if (vals == NULL)
    {
        vals = (seq_t *)malloc(16 * sizeof(seq_t));
        sz   = 0;
        next = 1;
    }

    /* extend cache until we have at least n terms */
    while (sz < (size_t)n)
    {
        idx_t k = next++;

        /* R(k) mod k */
        seq_t rmod = rep_mod_k(k);

        /* R(4) mod k is 1111 % k */
        uint32_t target = 1111U % (uint32_t)k;

        if (rmod == (seq_t)target)
        {
            if (sz == 0 || sz % 16 == 0)   /* grow cache */
            {
                size_t newcap = sz == 0 ? 16 : sz * 2;
                seq_t *tmp = (seq_t *)realloc(vals, newcap * sizeof(seq_t));
                vals = tmp;
            }
            vals[sz++] = (seq_t)k;
        }
    }

    return vals[n - 1];
}
