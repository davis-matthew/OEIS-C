/*
 * A212061  Number of (w,x,y,z) with all terms in {1,...,n} and w*x=(y*z)^2.
 * https://oeis.org/A212061
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{t=1}^{n} τ(t) * #{(w,x) | 1≤w,x≤n, w·x = t²}, where
 *            τ(t) is the number of divisors of t
 * Range:     u8 n <= 15, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   count_divs_t2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/count_divs_t2.h"

static uint32_t *spf_cache = NULL;
static idx_t spf_cache_limit = 0;

/* Build smallest prime factor table up to 'limit' */
static void ensure_spf(idx_t limit)
{
    if (limit <= spf_cache_limit)
        return;

    /* Reallocate table */
    uint32_t *new_spf = (uint32_t *)realloc(spf_cache, (limit + 1) * sizeof(uint32_t));
    if (!new_spf) { /* allocation failed, abort */
        exit(EXIT_FAILURE);
    }
    spf_cache = new_spf;

    /* Sieve */
    for (idx_t i = 0; i <= limit; ++i)
        spf_cache[i] = 0;
    if (limit >= 1)
        spf_cache[1] = 1;

    for (idx_t i = 2; i <= limit; ++i) {
        if (spf_cache[i] == 0) {
            spf_cache[i] = (uint32_t)i;
            if ((uint64_t)i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    if (spf_cache[j] == 0)
                        spf_cache[j] = (uint32_t)i;
            }
        }
    }
    spf_cache_limit = limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    ensure_spf(n);

    seq_t ans = 0;

    /* Temporary arrays for factorization */
    uint32_t primes[20];
    int exps[20];

    for (idx_t t = 1; t <= n; ++t) {
        /* Factorize t */
        idx_t tmp = t;
        int pcnt = 0;
        while (tmp > 1) {
            uint32_t p = spf_cache[tmp];
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                e++;
            }
            primes[pcnt] = p;
            exps[pcnt] = e;
            pcnt++;
        }

        /* τ(t) */
        seq_t tau_t = 1;
        for (int i = 0; i < pcnt; ++i)
            tau_t *= (seq_t)(exps[i] + 1);

        /* Count divisors of t² within bounds */
        seq_t t2 = (seq_t)t * (seq_t)t;
        seq_t cnt = 0;
        count_divs_t2(0, pcnt, primes, exps, (seq_t)1, t2, (seq_t)n, &cnt);

        ans += cnt * tau_t;
    }

    return ans;
}
