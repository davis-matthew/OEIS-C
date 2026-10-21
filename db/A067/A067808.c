/*
 * A067808  Numbers k such that sigma(k)^2 > 3*sigma(k^2).
 * https://oeis.org/A067808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer k such that σ(k)^2 > 3·σ(k^2)
 * Range:     u8 none, u16 n <= 328, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ---------- Sieve for smallest prime factor ---------- */
static unsigned int *spf = NULL;
static size_t spf_limit = 0;
static const size_t MAX_SIEVE = 5000000ULL;

static void init_spf(void)
{
    if (spf != NULL)
        return;                            /* already built */

    spf = (unsigned int*)malloc((MAX_SIEVE + 1) * sizeof(unsigned int));
    if (!spf)
        return;                            /* allocation failed */

    for (size_t i = 0; i <= MAX_SIEVE; ++i)
        spf[i] = 0;
    spf[0] = 0;
    spf[1] = 1;

    for (size_t i = 2; i <= MAX_SIEVE; ++i) {
        if (spf[i] == 0) {
            spf[i] = (unsigned int)i;
            if (i <= MAX_SIEVE / i) {
                for (size_t j = i * i; j <= MAX_SIEVE; j += i) {
                    if (spf[j] == 0)
                        spf[j] = (unsigned int)i;
                }
            }
        }
    }
    spf_limit = MAX_SIEVE;
}

/* ---------- Test whether k satisfies the inequality ---------- */
static int satisfies(uint64_t k)
{
    uint64_t n = k;
    __uint128_t sigma1 = 1;             /* σ(k) */
    __uint128_t sigma2 = 1;             /* σ(k²) */

    while (n > 1) {
        unsigned int p = spf[n];
        uint64_t e = 0;
        while (n % p == 0) {
            n /= p;
            ++e;
        }

        /* σ(p^e) = (p^(e+1)-1)/(p-1) */
        __uint128_t p_pow = 1;
        for (uint64_t i = 0; i < e + 1; ++i)
            p_pow *= p;
        __uint128_t sigma_pe = (p_pow - 1) / (p - 1);

        /* σ(p^(2e)) = (p^(2e+1)-1)/(p-1) */
        __uint128_t p_pow2 = 1;
        for (uint64_t i = 0; i < 2 * e + 1; ++i)
            p_pow2 *= p;
        __uint128_t sigma_p2e = (p_pow2 - 1) / (p - 1);

        sigma1 *= sigma_pe;
        sigma2 *= sigma_p2e;
    }

    __uint128_t left  = sigma1 * sigma1;
    __uint128_t right = sigma2 * 3;
    return left > right;
}

/* ---------- Cache for sequence values ---------- */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_k = 1;

static void cache_append(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return;                      /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* ---------- Main function requested by the problem statement ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                   /* undefined for 0-index */

    init_spf();
    while (cache_len < n) {
        if (satisfies(next_k))
            cache_append((seq_t)next_k);
        ++next_k;
    }
    return cache[n - 1];
}
