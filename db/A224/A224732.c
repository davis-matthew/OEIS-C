/*
 * A224732  G.f.: exp( Sum_{n>=1} binomial(2*n,n)^n * x^n/n ).
 * https://oeis.org/A224732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in exp( Σ_{k≥1} binomial(2k,k)^k * x^k
 *            / k ) Recurrence: a(0)=1; a(n) = (1/n) * Σ_{k=1..n}
 *            binomial(2k,k)^k * a(n-k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dependencies: ../utils/binomial.h , ../utils/big_pow.h */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/binomial.h"   /* seq_t binomial(idx_t a, idx_t b) */
#include "../utils/big_pow.h"    /* seq_t big_pow(seq_t base, seq_t exp) */

/* ensure the array can hold indices up to need-1; grow if necessary */
static void
ensure_cache(seq_t **arr, size_t *len, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    size_t newcap = need * 2 + 16;
    seq_t *newarr = (seq_t *)malloc(newcap * sizeof(seq_t));
    if (!newarr) return;
    if (*arr) {
        memcpy(newarr, *arr, (*len) * sizeof(seq_t));
        free(*arr);
    }
    memset(newarr + (*len), 0, (newcap - (*len)) * sizeof(seq_t));
    *arr  = newarr;
    *cap  = newcap;
}

seq_t a(idx_t n)
{
    static seq_t *a_cache = NULL;
    static seq_t *c_cache = NULL;           /* binomial(2k,k)^k */
    static size_t a_len = 0, a_cap = 0;
    static size_t c_len = 0, c_cap = 0;

    /* first call: initialise a(0)=1 */
    if (a_len == 0) {
        ensure_cache(&a_cache, &a_len, &a_cap, 1);
        a_cache[0] = (seq_t)1;
        a_len = 1;
    }

    size_t idx = (size_t)n;

    /* grow caches to include idx */
    if (idx + 1 > a_cap) ensure_cache(&a_cache, &a_len, &a_cap, idx + 1);
    if (idx + 1 > c_cap) ensure_cache(&c_cache, &c_len, &c_cap, idx + 1);

    /* compute missing terms up to idx */
    for (size_t N = a_len; N <= idx; ++N) {
        seq_t sum = (seq_t)0;
        for (size_t k = 1; k <= N; ++k) {
            /* compute c_cache[k] if not yet computed */
            if (k >= c_len) {
                seq_t bin = binomial((idx_t)(2 * k), (idx_t)k);
                seq_t pw  = big_pow(bin, (seq_t)k);       /* binom(2k,k)^k */
                c_cache[k] = pw;
                ++c_len;
            }
            sum += c_cache[k] * a_cache[N - k];
        }
        a_cache[N] = sum / (seq_t)N;   /* exact division */
    }

    /* a_len is already idx+1 after loop */
    return a_cache[idx];
}
