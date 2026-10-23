/*
 * A041290  Numerators of continued fraction convergents to sqrt(158).
 * https://oeis.org/A041290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(158)
   a(0) = 12, a(1) = 13, a(2) = 25, a(3) = 88, …,
   recurrence: p_{-1} = 1, p_0 = a0 = 12,
                p_n = a_n * p_{n-1} + p_{n-2}  (n ≥ 1),
   with periodic a_n (n ≥ 0):
     a0 = 12,
     a1.. = (1,1,3,12,3,1,1,24) repeating.
   The function only keeps a cache of computed terms. */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static const int period[] = {1, 1, 3, 12, 3, 1, 1, 24};
static const int period_len = 8;

seq_t a(idx_t n)
{
    /* initialise the cache with the first term a(0) = 12 */
    if (cache_len == 0) {
        cache_len = 1;
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* abort on OOM */
        cache[0] = (seq_t)12;
    }
    if (n < cache_len)
        return cache[n];

    /* prepare values for recurrence */
    seq_t p_prev_prev = 1;                 /* p_{-1} */
    seq_t p_prev = cache[cache_len - 1];   /* p_{cache_len-1} */

    if (cache_len >= 2) {
        p_prev_prev = cache[cache_len - 2];
    }

    /* extend the cache up to index n */
    while (cache_len <= n) {
        /* determine the continued‑fraction coefficient a_k */
        int ak_int;
        if (cache_len == 0) {
            ak_int = 12;
        } else {
            ak_int = period[(cache_len - 1) % period_len];
        }
        seq_t ak = (seq_t)ak_int;

        /* compute the next numerator */
        seq_t p_new = ak * p_prev + p_prev_prev;

        /* store it */
        seq_t *tmp = (seq_t*)realloc(cache, (cache_len + 1) * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* abort on OOM */
        cache = tmp;
        cache[cache_len] = p_new;

        /* update for next step */
        p_prev_prev = p_prev;
        p_prev = p_new;
        ++cache_len;
    }

    return cache[n];
}
