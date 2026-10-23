/*
 * A036413  Values of k for which there are no empty intervals when
 *          fractional_part(m*e) for m = 1, ..., k is plotted along [ 0, 1 ]
 *          subdivided into k equal regions.
 * https://oeis.org/A036413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = the n‑th element of the sequence of denominators of the
 *            continued‑fraction convergents of e, with the early extra terms
 *            2, 6 and 35 inserted at the correct positions
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache to allow out‑of‑order calls */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static bool initialized = false;
    /* recurrence state */
    static idx_t next_cf = 1;                /* index of next partial quotient to process */
    static seq_t p_prev_prev, p_prev, q_prev_prev, q_prev;

    if (!initialized) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;

        /* first two terms */
        cache[cache_len++] = (seq_t)1;  /* q0 */
        cache[cache_len++] = (seq_t)2;  /* extra element after q0 */

        /* initialise convergent recurrence */
        p_prev_prev = (seq_t)1;    /* p_{-1} */
        p_prev     = (seq_t)2;    /* p_0   */
        q_prev_prev = (seq_t)0;   /* q_{-1} */
        q_prev     = (seq_t)1;    /* q_0   */

        next_cf = 1;              /* the next partial quotient to process */
        initialized = true;
    }

    /* produce terms until we have at least n of them */
    while (cache_len < n) {
        /* partial quotient of e: a0 = 2, a_k = 1 if k%3≠2, else 2*((k+1)/3) */
        seq_t a_val;
        if (next_cf % 3 == 2) {
            a_val = (seq_t)(2 * ((next_cf + 1) / 3));
        } else {
            a_val = (seq_t)1;
        }

        /* compute next convergent */
        seq_t p_new = a_val * p_prev + p_prev_prev;
        seq_t q_new = a_val * q_prev + q_prev_prev;

        if (next_cf != 1) {        /* skip the duplicate 1 (q1)          */
            /* ensure capacity */
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = q_new;   /* push the denominator */

            /* insert special terms at the proper positions */
            if (next_cf == 3) {          /* after q3 */
                if (cache_len == cache_cap) {
                    cache_cap *= 2;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_len++] = (seq_t)6;
            }
            if (next_cf == 5) {          /* after q5 */
                if (cache_len == cache_cap) {
                    cache_cap *= 2;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_len++] = (seq_t)35;
            }
        }

        /* shift recurrence state */
        p_prev_prev = p_prev;
        p_prev = p_new;
        q_prev_prev = q_prev;
        q_prev = q_new;

        ++next_cf;  /* move to next partial quotient */
    }

    return cache[n - 1];
}
