/*
 * A042254  Numerators of continued fraction convergents to sqrt(653).
 * https://oeis.org/A042254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 37, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numerators of continued fraction convergents to sqrt(653).
   Recurrence: let a0 = 25; let a_n for n≥1 be the periodic
   continued‑fraction coefficients of sqrt(653).  Define
   p_-2 = 0, p_-1 = 1, and
   p_n = a_n * p_{n-1} + p_{n-2}.
   Then a(n) = p_n. */
#include <stdlib.h>

static idx_t period[512];
static size_t period_len = 0;

static void init_period(void) {
    if (period_len != 0) return;
    const idx_t D = 653;
    idx_t a0 = 0;
    while ((a0 + 1) * (a0 + 1) <= D) ++a0;
    idx_t m = 0, d = 1, a = a0;
    do {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;
        period[period_len++] = a;
    } while (!(m == a0 && d == 1));
}

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static const seq_t sqrt653_floor = (seq_t)25;

static void ensure_cache(idx_t n) {
    init_period();
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    }
    if ((n + 1) > cache_cap) {
        while (cache_cap <= n + 1) cache_cap <<= 1;
        cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
    }
    if (cache_len == 0) {
        cache[0] = sqrt653_floor;
        cache_len = 1;
    }
    if (cache_len == 1 && n >= 1) {
        seq_t a1 = (seq_t)period[0];
        cache[1] = a1 * cache[0] + (seq_t)1;
        cache_len = 2;
    }
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t ai = (seq_t)period[(i - 1) % period_len];
        cache[i] = ai * cache[i - 1] + cache[i - 2];
    }
    cache_len = n + 1;
}

seq_t a(idx_t n) {
    ensure_cache(n);
    return cache[n];
}
