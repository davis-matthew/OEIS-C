/*
 * A086331  Expansion of e.g.f. exp(x)/(1 + LambertW(-x)).
 * https://oeis.org/A086331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * k^k
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"

/* Static cache for computed values of a(n) */
static seq_t *a_cache = NULL;
static size_t a_cap   = 0;      /* allocated capacity */
static size_t a_len   = 0;      /* number of values stored */

/* Static cache for pre‑computed k^k values */
static seq_t *pow_cache = NULL;
static size_t pow_cap   = 0;    /* allocated capacity */
static size_t pow_len   = 0;    /* number of values stored */

/* Ensure the cache for a(n) has enough space */
static void ensure_a_capacity(size_t needed)
{
    if (needed <= a_cap) return;
    size_t new_cap = needed * 2;
    seq_t *new_ptr =
#ifdef __cplusplus
        new seq_t[new_cap];
#else
        (seq_t*)malloc(sizeof(seq_t) * new_cap);
#endif
    if (a_cache) {
        for (size_t i = 0; i < a_len; ++i) new_ptr[i] = a_cache[i];
#ifdef __cplusplus
        delete[] a_cache;
#else
        free(a_cache);
#endif
    }
    a_cache = new_ptr;
    a_cap   = new_cap;
}

/* Ensure the cache for k^k values has enough space */
static void ensure_pow_capacity(size_t needed)
{
    if (needed <= pow_cap) return;
    size_t new_cap = needed * 2;
    seq_t *new_ptr =
#ifdef __cplusplus
        new seq_t[new_cap];
#else
        (seq_t*)malloc(sizeof(seq_t) * new_cap);
#endif
    if (pow_cache) {
        for (size_t i = 0; i < pow_len; ++i) new_ptr[i] = pow_cache[i];
#ifdef __cplusplus
        delete[] pow_cache;
#else
        free(pow_cache);
#endif
    }
    pow_cache = new_ptr;
    pow_cap   = new_cap;
}

/* Compute any missing k^k values up to n */
static void compute_pows(idx_t n)
{
    if ((size_t)n + 1 <= pow_len) return;
    size_t start = pow_len;
    ensure_pow_capacity((size_t)n + 1);
    for (idx_t k = (idx_t)start; k <= n; ++k) {
        pow_cache[(size_t)k] = big_pow((seq_t)k, (seq_t)k);
    }
    pow_len = (size_t)n + 1;
}

/* Compute a(n) for all n up to desired value */
static void compute_up_to(idx_t n)
{
    if (n < a_len) return;
    size_t start = a_len;
    ensure_a_capacity((size_t)n + 1);
    compute_pows(n);
    for (idx_t i = (idx_t)start; i <= n; ++i) {
        seq_t sum   = 0;
        seq_t binom = 1;
        for (idx_t k = 0; k <= i; ++k) {
            seq_t term = seq_mul(binom, pow_cache[(size_t)k]);
            sum += term;
            if (k < i) {
                seq_t t = seq_mul(binom, (seq_t)(i - k));
                binom   = t / (seq_t)(k + 1);
            }
        }
        a_cache[(size_t)i] = sum;
    }
    a_len = (size_t)n + 1;
}

/* Public function required by the build system */
seq_t a(idx_t n)
{
    compute_up_to(n);
    return a_cache[(size_t)n];
}
