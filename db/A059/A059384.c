/*
 * A059384  a(n) = Product_{i=1..n} J_5(i).
 * https://oeis.org/A059384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i=1..n} J_5(i) (Jordan totient with k = 5)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ---- small integer power for seq_t ---- */
static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t res = 1;
    while (exp > 0) {
        if (exp & 1U) res *= base;
        exp >>= 1U;
        if (exp) base *= base;
    }
    return res;
}

/* ---- smallest prime factor table ---- */
static uint64_t *spf  = NULL;      /* spf[i] = smallest prime factor of i, 1 <= i <= spf_limit */
static idx_t    spf_limit = 0;

/* Build/spf up to 'limit' (inclusive) */
static void build_spf(idx_t limit)
{
    if (limit <= spf_limit) return;               /* already built up to this limit */

    uint64_t *new_spf = (uint64_t *)malloc((size_t)(limit + 1) * sizeof(uint64_t));
    if (!new_spf) return;                          /* allocation failed – caller will get 0 */

    for (idx_t i = 0; i <= limit; ++i) new_spf[i] = 0;
    new_spf[1] = 1;

    for (idx_t i = 2; i <= limit; ++i) {
        if (new_spf[i] == 0) {                     /* i is prime */
            new_spf[i] = i;
            if (i <= limit / i) {
                for (idx_t j = i * i; j <= limit; j += i)
                    if (new_spf[j] == 0) new_spf[j] = i;
            }
        }
    }

    /* free old table, install new one */
    free(spf);
    spf = new_spf;
    spf_limit = limit;
}

/* ---- cache for a(n) ---- */
static seq_t *cache      = NULL; /* cache[0..max_cached], cache[0] = 1 (dummy) */
static idx_t  max_cached = 0;

/* Helper: compute J_5(n) given prime factor exponents */
static seq_t jordan5(idx_t n)
{
    if (n == 1) return (seq_t)1;          /* J_5(1) = 1 */

    seq_t prod = 1;
    while (n > 1) {
        uint64_t p = spf[n];
        idx_t exp = 0;
        while (n % p == 0) {
            n /= p;
            exp++;
        }

        /* factor = p^{5*exp} - p^{5*(exp-1)} */
        seq_t p_pow5minus1 = pow_seq((seq_t)p, 5);             /* p^5 */
        seq_t factor;

        if (exp == 1) {
            factor = p_pow5minus1 - 1;                       /* p^5 - 1 */
        } else {
            seq_t p_pow5e_minus1 = pow_seq((seq_t)p, 5 * (exp - 1)); /* p^{5*(e-1)} */
            factor = p_pow5minus1 * p_pow5e_minus1 - p_pow5e_minus1;
        }
        prod *= factor;
    }
    return prod;
}

/* ---- main sequence function ---- */
seq_t a(idx_t n)
{
    if (n == 0) return 1;                 /* offset 1 – a(0) unused, return dummy */

    /* Build prime factor table up to n */
    build_spf(n);

    /* Grow cache if necessary */
    if (n > max_cached) {
        idx_t new_size = (idx_t)(n + 1);   /* include index 0 */
        seq_t *new_cache = (seq_t *)malloc((size_t)new_size * sizeof(seq_t));
        if (!new_cache) return 0;          /* allocation failure */

        /* initialize dummy at index 0 */
        new_cache[0] = 1;

        /* copy old values */
        for (idx_t i = 1; i <= max_cached; ++i) new_cache[i] = cache[i];

        /* compute missing values */
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            seq_t j5 = jordan5(i);
            new_cache[i] = new_cache[i - 1] * j5;
        }

        /* replace old cache */
        free(cache);
        cache = new_cache;
        max_cached = n;
    }
    return cache[n];
}
