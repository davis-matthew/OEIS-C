/*
 * A180606  G.f.: A(x) = exp( Sum_{n>=1} (2^n-1)^(n-1)*x^n/n ).
 * https://oeis.org/A180606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>=1, a(n) = (1/n) * Σ_{k=1}^{n} (2^k-1)^{k-1} *
 *            a(n-k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   big_pow, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/pow2.h"

static seq_t *a_cache = NULL;       /* a-values */
static size_t a_cap  = 0;           /* allocated size of a_cache */
static size_t a_used = 0;           /* max index computed+1 */

static seq_t *p_cache = NULL;       /* (2^k-1)^(k-1) values */
static size_t p_cap  = 0;
static size_t p_used = 0;

/* Resize the caches if necessary */
static void ensure_cap(idx_t n)
{
    size_t need = (size_t)(n + 1);

    if (a_cap < need) {
        a_cache = (seq_t *)realloc(a_cache, need * sizeof(seq_t));
        a_cap   = need;
    }
    if (p_cap < need) {
        p_cache = (seq_t *)realloc(p_cache, need * sizeof(seq_t));
        p_cap   = need;
    }
}

/* Compute p_cache[k] = (2^k-1)^(k-1) for all k up to n */
static void compute_p_up_to(idx_t n)
{
    for (idx_t k = (idx_t)(p_used + 1); k <= n; ++k) {
        seq_t base = pow2(k);     /* 2^k */
        base -= 1;                /* 2^k - 1 */
        seq_t exp  = (seq_t)(k - 1);
        p_cache[k] = big_pow(base, exp);
    }
    if (p_used < (size_t)n) p_used = (size_t)n;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        if (a_used == 0) {
            ensure_cap(0);
            a_cache[0] = 1;          /* a(0) = 1 */
            a_used = 1;
        }
        return a_cache[0];
    }

    ensure_cap(n);

    /* Compute any missing a-values up to n */
    for (size_t idx = a_used; idx <= (size_t)n; ++idx) {
        idx_t i = (idx_t)idx;

        /* Ensure necessary p-values are available */
        compute_p_up_to(i);

        seq_t sum = 0;
        for (idx_t k = 1; k <= i; ++k) {
            seq_t prod = p_cache[k] * a_cache[i - k];
            sum += prod;
        }
        a_cache[i] = sum / (seq_t)i;   /* division is exact integer */
    }

    a_used = (size_t)n + 1;
    return a_cache[n];
}
