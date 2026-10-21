/*
 * A052653  E.g.f. (1-2x^2)/(1-x-2x^2).
 * https://oeis.org/A052653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A052653: E.g.f. (1-2x^2)/(1-x-2x^2) */
/* Recurrence (for n>=1):
   a(n+2) = (2n^2 + 6n + 4)*a(n) + (n+2)*a(n+1)
   with initial values a(0)=1, a(1)=1, a(2)=2.
   The recurrence holds for n>=1; terms 0,1,2 are given. */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;     /* memoization array for a(n) */
static idx_t cached = 0;        /* number of terms stored (cache[0..cached-1]) */
static size_t capacity = 0;     /* allocated capacity of cache */

/* helper to ensure cache has space for at least 'need' terms (need > capacity) */
static void ensure_capacity(size_t need)
{
    if (capacity >= need)
        return;
    size_t new_cap = capacity ? capacity * 2 : 4;
    if (new_cap < need)
        new_cap = need;
    seq_t *new_cache = NULL;
    new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) abort();
    if (cache) {
        for (size_t i = 0; i < cached; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }
    cache = new_cache;
    capacity = new_cap;
}

seq_t a(idx_t n)
{
    /* Handle base cases lazily while initializing cache */
    if (cache == NULL) {
        ensure_capacity(3);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cached = 3;
    }

    if (n < cached)
        return cache[n];

    /* Extend cache up to n using the recurrence */
    for (idx_t i = cached; i <= n; ++i)
    {
        seq_t val;
        if (i == 0) {
            val = (seq_t)1;
        } else if (i == 1) {
            val = (seq_t)1;
        } else if (i == 2) {
            val = (seq_t)2;
        } else {
            /* Recurrence: a_i = (2*(i-2)^2 + 6*(i-2) + 4)*a_{i-2} + (i-2+2)*a_{i-1} */
            idx_t k = i - 2;                 /* n in the recurrence formula */
            seq_t a_k   = cache[k];
            seq_t a_k1  = cache[k + 1];

            /* Compute coefficients using 128‑bit intermediates to avoid overflow */
            unsigned __int128 temp = (unsigned __int128)k * k;
            unsigned long long coeff1 = (unsigned long long)(2 * temp + 6 * k + 4);
            unsigned long long coeff2 = (unsigned long long)(k + 2);

            val = (seq_t)coeff1 * a_k + (seq_t)coeff2 * a_k1;
        }
        /* Grow cache if needed before storing the new value */
        if (i >= capacity)
            ensure_capacity(i + 1);
        cache[i] = val;
    }
    cached = (n + 1) > cached ? (n + 1) : cached;
    return cache[n];
}
