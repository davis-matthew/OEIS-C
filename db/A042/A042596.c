/*
 * A042596  Numerators of continued fraction convergents to sqrt(827).
 * https://oeis.org/A042596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(827).
   The sequence satisfies the recurrence
       p_(-1) = 1,   p_0 = 28
       p_k   = a_k * p_(k-1) + p_(k-2)  for k >= 1
   where the partial quotients a_k (k ≥ 1) are periodic with
   period [1, 3, 7, 1, 27, 1, 7, 3, 1, 56].
   a(0) = p_0.  */

#include <stdlib.h>

static seq_t *cache = NULL;    /* cached terms of the sequence   */
static size_t cache_cap = 0;   /* capacity of the cache          */
static idx_t cache_len = 0;    /* number of terms currently stored */

/* Periodic partial quotients after the initial 28. */
static const int period[10] = {1, 3, 7, 1, 27, 1, 7, 3, 1, 56};

seq_t a(idx_t n)
{
    /* Grow the cache if necessary. */
    if (cache_len == 0) {
        cache_cap = 32;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 32;
        while (new_cap < (size_t)(n + 1)) new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Generate terms up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)28;          /* p_0 */
            continue;
        }

        seq_t prevprev = (i == 1) ? (seq_t)1 : cache[i - 2]; /* p_(i-2) */
        seq_t prev     = cache[i - 1];                         /* p_(i-1) */
        seq_t coeff    = (seq_t)period[(i - 1) % 10];          /* a_i */

        cache[i] = coeff * prev + prevprev;                    /* p_i   */
    }

    cache_len = n + 1;
    return cache[n];
}
