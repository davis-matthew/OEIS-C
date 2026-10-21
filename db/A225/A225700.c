/*
 * A225700  Denominators of coefficients arising from q-expansion of
 *          Integrate[eta[q^4]^8/eta[q^2]^4, q]/q where eta is the Dedekind
 *          eta function.
 * https://oeis.org/A225700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   For n >= 0 let k = 2*n+1. Let σ(k) be the sum of all divisors of
 *            k (k is odd, so all divisors are odd). Let d = 2*(n+1). Then
 *            a(n) = d / gcd(σ(k), d) This yields the denominators of the
 *            coefficients of the q‑expansion described in OEIS A225700
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static uint32_t *spf = NULL;          /* smallest prime factor array   */
static idx_t   spf_cap = 0;           /* current capacity of spf array */

/* Build the smallest‑prime‑factor sieve up to 'limit' (inclusive).  */
static void build_spf(idx_t limit)
{
    if (limit <= spf_cap) return;
    uint32_t new_cap = (uint32_t)limit + 1;
    uint32_t *new_spf = (uint32_t *)malloc(new_cap * sizeof(uint32_t));
    if (!new_spf) abort();
    for (uint32_t i = 0; i < new_cap; ++i) new_spf[i] = 0;

    for (uint32_t i = 2; (uint64_t)i * i < new_cap; ++i) {
        if (new_spf[i] == 0) {
            new_spf[i] = i;
            for (uint64_t j = (uint64_t)i * i; j < new_cap; j += i) {
                if (new_spf[(uint32_t)j] == 0) new_spf[(uint32_t)j] = i;
            }
        }
    }
    for (uint32_t i = 2; i < new_cap; ++i)
        if (new_spf[i] == 0) new_spf[i] = i;

    free(spf);
    spf = new_spf;
    spf_cap = new_cap;
}

/* Greatest common divisor of seq_t values.  */
static seq_t seq_gcd_local(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Sum of all divisors of the odd integer 'n'.  */
static seq_t sigma_of(idx_t n)
{
    build_spf(n);
    idx_t tmp = n;
    seq_t result = (seq_t)1;

    while (tmp > (idx_t)1) {
        uint32_t p = spf[(uint32_t)tmp];
        int cnt = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++cnt;
        }
        seq_t power = (seq_t)1;
        seq_t sum_pow = (seq_t)1;
        for (int i = 0; i < cnt; ++i) {
            power *= (seq_t)p;
            sum_pow += power;
        }
        result *= sum_pow;
    }
    return result;
}

/* Cache of already computed terms.  */
static seq_t *cache = NULL;
static idx_t  cache_cap = 0;

/* Grow the cache to include at least indices 0..n.  */
static void ensure_cache(idx_t n)
{
    if (cache_cap > n) return;
    idx_t old_cap = cache_cap;
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;
    cache_cap = n + 1;

    for (idx_t i = old_cap; i < cache_cap; ++i) {
        idx_t k = (idx_t)2 * i + (idx_t)1;          /* odd integer  */
        uint32_t m = (uint32_t)(i + 1);             /* (k+1)/2      */
        seq_t denom = (seq_t)2 * (seq_t)m;          /* 2*(n+1)      */
        seq_t num   = sigma_of(k);                  /* σ(k)         */
        seq_t g     = seq_gcd_local(num, denom);
        cache[i]    = denom / g;                    /* reduced denominator */
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
