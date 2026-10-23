/*
 * A041796  Numerators of continued fraction convergents to sqrt(419).
 * https://oeis.org/A041796
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 17, u64 n <= 37, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A041796: Numerators of continued fraction convergents to sqrt(419).
 *
 * Formula:  a(0) = 20
 *           a(1) = 41
 *           for n ≥ 2: a(n) = c[n] * a(n-1) + a(n-2)
 *           where the periodic coefficients c[n] are the continued‑fraction
 *           partial quotients of sqrt(419) after a0:
 *           c[n] = [2,7,1,2,3,1,2,1,19,1,2,1,3,2,1,7,2,40] repeated.
 *
 * The implementation precomputes these coefficients once, then
 * memoises the sequence values in a dynamic array.
 *
 * Functions and types required by the build system are assumed to exist
 * (seq_t, idx_t, etc.).
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* static state */
static size_t cache_len = 0;                /* number of terms currently stored */
static size_t cache_cap = 0;                /* current allocated capacity */
static seq_t *cache = NULL;                 /* array of computed terms */

static idx_t period_len = 0;                /* length of repeating coefficient block */
static uint64_t *coeffs = NULL;             /* array of coefficients c[0..period_len-1] */

/* compute and store the continued fraction coefficients of sqrt(419) */
static void init_coefficients(void)
{
    if (period_len != 0)
        return;                            /* already initialised */

    const idx_t D = 419;
    const idx_t a0 = 20;
    idx_t m = 0, d = 1, a = a0;

    /* Period for sqrt(419) is 18; allocate a bit more for safety */
    size_t max_est = 64;
    coeffs = (uint64_t *)malloc(max_est * sizeof(uint64_t));
    if (!coeffs)
        return;                            /* allocation failed – not expected */

    size_t idx = 0;
    do {
        m = d * a - m;                      /* m_k+1 = d_k * a_k - m_k */
        d = (D - m * m) / d;                /* d_k+1 = (D - m_k+1²) / d_k */
        a = (a0 + m) / d;                   /* a_k+1 = floor((a0 + m_k+1) / d_k+1) */
        coeffs[idx++] = a;
    } while (!(m == a0 && d == 1));          /* stop when we return to start */

    period_len = (idx_t)idx;                    /* should be 18 */
}

/* ensure the cache array has capacity for terms up to 'needed' (inclusive) */
static void ensure_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = needed;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        return;                            /* allocation failed – not expected */

    /* copy existing computed values */
    if (cache != NULL) {
        for (size_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }

    cache = new_cache;
    cache_cap = new_cap;
}

/* External function required by the evaluation harness */
seq_t a(idx_t n)
{
    /* initialise coefficients once */
    if (period_len == 0)
        init_coefficients();

    /* handle extent of cache */
    size_t n_usize = (size_t)n;
    ensure_cache(n_usize + 1);

    /* if the requested term is already computed return it */
    if (n_usize < cache_len)
        return cache[n_usize];

    /* compute missing terms from cache_len up to n */
    for (size_t i = cache_len; i <= n_usize; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)20;
        } else if (i == 1) {
            seq_t coeff = (seq_t)coeffs[0];          /* coeff for a1 */
            cache[1] = coeff * cache[0] + (seq_t)1;
        } else {
            seq_t coeff = (seq_t)coeffs[(i - 1) % period_len];
            cache[i] = coeff * cache[i - 1] + cache[i - 2];
        }
    }

    cache_len = n_usize + 1;
    return cache[n_usize];
}
