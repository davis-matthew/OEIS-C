/*
 * A158949  Inverse Moebius transform of A065958.
 * https://oeis.org/A158949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, mult, nonn
 * Formula:   a(n) = ∏_{p^e‖n} (p^(2e)*(p^2+1) – 2) / (p^2 – 1), a(1)=1 where
 *            the product runs over the distinct prime powers p^e dividing n
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 3505, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* exponentiation for seq_t */
static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t res = 1;
    while (exp)
    {
        if (exp & 1U)
            res *= base;
        base *= base;
        exp >>= 1U;
    }
    return res;
}

/* value of a(p^e) for a prime p and exponent e */
static inline seq_t a_prime_power(seq_t p, idx_t e)
{
    seq_t p2   = p * p;                      /* p^2 */
    seq_t pw   = pow_seq(p, 2*e);            /* p^(2e) */
    pw = pw * (p2 + 1) - 2;                  /* numerator */
    seq_t denom = p2 - 1;                    /* denominator */
    return pw / denom;                       /* exact division */
}

/* cache for already computed terms */
static seq_t *cache      = NULL;   /* 1‑based array of a(n) */
static idx_t  cache_cap   = 0;     /* allocated size (index of last element) */

/* ensure that a(n) is computed for all 1 ≤ k ≤ n */
static void ensure_up_to(idx_t n)
{
    if (n < 1) return;

    /* enlarge cache if necessary */
    if (n + 1 > cache_cap)
    {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) { /* allocation failed – keep empty cache */
            cache   = NULL;
            cache_cap = 0;
            return;
        }
        for (idx_t i = cache_cap; i <= n; ++i)
            tmp[i] = 0;                 /* initialise new entries */
        cache = tmp;
        cache_cap = n + 1;
    }

    /* compute missing entries */
    idx_t start = (cache_cap > 0 && cache[1] != 0) ? 0 : 1;
    /* if we have not set a(1) yet, start from 1 */
    for (idx_t x = (cache[1] == 0 ? 1 : 1); x <= n; ++x)
    {
        if (x == 1)
        {
            cache[1] = 1;
            continue;
        }
        uint64_t v = (uint64_t)x;
        seq_t prod = 1;

        /* trial division factorisation */
        for (uint64_t p = 2; p * p <= v; ++p)
        {
            if (v % p != 0) continue;
            idx_t e = 0;
            while (v % p == 0)
            {
                v /= p;
                ++e;
            }
            prod *= a_prime_power((seq_t)p, e);
        }
        if (v > 1)          /* remaining prime factor */
        {
            prod *= a_prime_power((seq_t)v, 1);
        }
        cache[x] = prod;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined, but safe */
    ensure_up_to(n);
    return cache[n];
}
