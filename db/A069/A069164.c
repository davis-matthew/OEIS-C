/*
 * A069164  Numbers k such that k*Sum_{d|k} 1/tau(d) is an integer.
 * https://oeis.org/A069164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   Numbers k such that k*Sum_{d|k} 1/tau(d) is an integer, where
 *            tau(d) is the number of divisors of d. The sequence is generated
 *            by testing each integer k in ascending order and keeping those
 *            for which k * S(k) is integral. The function builds the sequence
 *            on demand, caching results for future calls
 * Range:     u8 n <= 6, u16 n <= 31, u32 n <= 3814, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_gcd.h"

/* Precompute the harmonic numbers H_m = 1 + 1/2 + ... + 1/m as reduced
   fractions.  We need H_{e+1} for exponents e occurring in k. */
#define MAX_EXP 64
static seq_t h_num[MAX_EXP + 1];
static seq_t h_den[MAX_EXP + 1];
static bool h_precomp = false;

static void precompute_h(void)
{
    if (h_precomp) return;
    h_num[1] = 1;
    h_den[1] = 1;                 /* H1 = 1 */
    for (int m = 2; m <= MAX_EXP; ++m) {
        seq_t prev_num = h_num[m - 1];
        seq_t prev_den = h_den[m - 1];
        /* H_m = H_{m-1} + 1/m = (prev_num* m + prev_den) / (prev_den * m) */
        seq_t new_num = prev_num * (seq_t)m + prev_den;
        seq_t new_den = prev_den * (seq_t)m;
        seq_t g = seq_gcd(new_num, new_den);
        h_num[m] = new_num / g;
        h_den[m] = new_den / g;
    }
    h_precomp = true;
}

/* Simple sieve to compute smallest prime factor for all numbers up to a
   fixed limit.  1,000,000 is sufficient for the first 10,000 terms. */
#define SPF_LIMIT 1000000
static uint32_t spf[SPF_LIMIT + 1];
static bool spf_built = false;

static void build_spf(void)
{
    if (spf_built) return;
    for (uint32_t i = 0; i <= SPF_LIMIT; ++i) spf[i] = 0;
    for (uint32_t i = 2; i <= SPF_LIMIT; ++i) {
        if (!spf[i]) {
            spf[i] = i;
            if ((uint64_t)i * i <= SPF_LIMIT) {
                for (uint64_t j = (uint64_t)i * i; j <= SPF_LIMIT; j += i)
                    if (!spf[j]) spf[j] = i;
            }
        }
    }
    spf_built = true;
}

/* Return true iff k satisfies the condition. */
static bool qualifies(idx_t k)
{
    idx_t n = k;
    seq_t num = 1;   /* numerator of the product of H_{e+1} */
    seq_t den = 1;   /* denominator of the product of H_{e+1} */

    while (n > 1) {
        uint32_t p = spf[n];
        int e = 0;
        while (n % p == 0) {
            n /= p;
            ++e;
        }
        seq_t h_n = h_num[e + 1];
        seq_t h_d = h_den[e + 1];
        num = num * h_n;
        den = den * h_d;
        seq_t g = seq_gcd(num, den);
        num /= g;
        den /= g;
    }

    seq_t prod = (seq_t)k * num;
    return (prod % den == 0);
}

/* Main entry point for the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* the index is 1-based; n>=1 per spec */

    static seq_t *cache = NULL;     /* cached sequence values */
    static size_t cache_len = 0;    /* number of values stored */
    static size_t cache_cap = 0;    /* allocated capacity */
    static idx_t last_checked = 0;  /* largest integer examined so far */

    /* One-time initialization. */
    if (cache_len == 0) {
        precompute_h();
        build_spf();
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Generate values up to the requested index. */
    while (cache_len < n) {
        ++last_checked;
        if (qualifies(last_checked)) {
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1];
}
