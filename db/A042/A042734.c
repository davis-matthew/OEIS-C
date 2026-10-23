/*
 * A042734  Numerators of continued fraction convergents to sqrt(897).
 * https://oeis.org/A042734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* PE: A042734 – Numerators of continued fraction convergents to sqrt(897) */
#include <stdlib.h>

extern seq_t a(idx_t n);

/* Recurrence: a(n) = 1198*a(n-4) - a(n-8)
   Initial values: a(0)=29, a(1)=30, a(2)=569, a(3)=599,
                    a(4)=35311, a(5)=35910, a(6)=681691, a(7)=717601 */
static const seq_t init_terms[8] = {
    (seq_t)29,      (seq_t)30,      (seq_t)569,     (seq_t)599,
    (seq_t)35311,   (seq_t)35910,   (seq_t)681691,  (seq_t)717601
};

static seq_t *cache = NULL;     /* array of computed values          */
static size_t  cache_cap = 0;   /* current allocated capacity         */
static size_t  cache_len = 0;   /* last computed index + 1           */

static void ensure_capacity(size_t needed)
{
    if (needed < cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap <= needed) new_cap <<= 1;      /* double until big enough */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return;                           /* allocation failed – keep old */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Initialise cache on first call */
    if (!cache) {
        ensure_capacity(15);                    /* enough for n up to 15 initially */
        for (size_t i = 0; i < 8; ++i) cache[i] = init_terms[i];
        cache_len = 8;
    }

    /* Grow cache to cover requested index */
    size_t idx = (size_t)n;
    ensure_capacity(idx);

    /* Compute missing terms up to idx */
    while (cache_len <= idx) {
        size_t i = cache_len;
        /* recurrence: a(i) = 1198*a(i-4) - a(i-8) */
        seq_t term = (seq_t)1198 * cache[i - 4] - cache[i - 8];
        cache[i] = term;
        ++cache_len;
    }

    return cache[idx];
}
