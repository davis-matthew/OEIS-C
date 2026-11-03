/*
 * A155202  G.f.: A(x) = exp( Sum_{n>=1} (2^n - 1)^n * x^n/n ), a power series
 *          in x with integer coefficients.
 * https://oeis.org/A155202
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = (1/n) * Σ_{k=1..n} (2^k - 1)^k * a(n-k) for n ≥
 *            1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 11 (2026-11-03)
 * Helpers:   ipow, mem, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow.h"
#include "../utils/pow2.h"
#include "../utils/mem.h"

static seq_t *a_cache = NULL;   /* cache of a(k), size a_size */
static idx_t  a_size  = 0;

static seq_t *t_cache = NULL;   /* cache of (2^k - 1)^k, size t_size */
static idx_t  t_size  = 0;

/* Ensure t_cache holds values up to index k (inclusive). */
static void
ensure_t_cache(idx_t k)
{
    if (k < t_size)
        return;

    idx_t new_size = k + 1;
    seq_t *tmp = mem_resize(seq_t, t_cache, new_size);
    if (!tmp)
        abort();
    t_cache = tmp;
    for (idx_t i = t_size; i <= k; ++i) {
        seq_t two_pow = pow2(i);
        seq_t base = two_pow - (seq_t)1;
        t_cache[i] = ipow(base, (seq_t)i);
    }
    t_size = new_size;
}

/* Ensure a_cache holds values up to index n (inclusive). */
static void
ensure_a_cache(idx_t n)
{
    if (a_cache == NULL) {            /* first call: allocate and set a(0) */
        a_cache = mem_new(seq_t, 1);
        if (!a_cache)
            abort();
        a_cache[0] = (seq_t)1;
        a_size = 1;
    }

    if (n < a_size)
        return;

    idx_t new_size = n + 1;
    seq_t *tmp = mem_resize(seq_t, a_cache, new_size);
    if (!tmp)
        abort();
    a_cache = tmp;

    for (idx_t i = a_size; i <= n; ++i) {
        ensure_t_cache(i);
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= i; ++k) {
            sum += t_cache[k] * a_cache[i - k];
        }
        a_cache[i] = sum / (seq_t)i;   /* division is exact by construction */
    }
    a_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    ensure_a_cache(n);
    return a_cache[n];
}
