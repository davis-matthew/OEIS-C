/*
 * A042761  Denominators of continued fraction convergents to sqrt(911).
 * https://oeis.org/A042761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 17, u64 n <= 30, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of the continued‑fraction convergents to sqrt(911)   
   a(0) = 1, a(1) = 5, a(n) = a_cf(n) · a(n‑1) + a(n‑2) for n ≥ 2,  
   where a_cf(n) are the partial quotients of sqrt(911). */

#include <stdlib.h>
#include <stdint.h>

static const uint64_t D = 911;             /* the number under the square root */
static const uint64_t A0 = 30;             /* floor(sqrt(911)) */

/* Periodic continued‑fraction coefficients (excluding the initial a0). */
static seq_t *period = NULL;
static size_t period_len = 0;

/* Cache of computed denominators. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_size = 0;

/* Compute the periodic part of the continued fraction of sqrt(D). */
static void init_period(void)
{
    if (period != NULL)
        return;

    /* a small initial capacity; it will grow if needed. */
    size_t cap = 64;
    period = (seq_t *)malloc(cap * sizeof(seq_t));
    size_t len = 0;

    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t a = A0;

    for (;;) {
        /* Next partial quotient */
        m = d * a - m;
        d = (D - m * m) / d;
        a = (A0 + m) / d;

        /* Store a_i into the period vector */
        if (len == cap) {
            cap *= 2;
            period = (seq_t *)realloc(period, cap * sizeof(seq_t));
        }
        period[len++] = (seq_t)a;

        /* The period ends when a_i == 2*A0 */
        if (a == 2 * A0)
            break;
    }
    period_len = len;
}

/* Expand the cache of denominators up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n < cache_size)
        return;

    /* Ensure enough capacity in the cache array. */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap == 0 ? 16 : cache_cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute missing terms. */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i == 1) {
            cache[1] = period[0];
        } else {
            seq_t a_i = period[((size_t)(i - 1)) % period_len];
            cache[i] = a_i * cache[i - 1] + cache[i - 2];
        }
    }
    cache_size = (size_t)n + 1;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    init_period();
    ensure_cache(n);
    return cache[n];
}
