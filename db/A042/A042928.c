/*
 * A042928  Numerators of continued fraction convergents to sqrt(996).
 * https://oeis.org/A042928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the n‑th convergent of the continued
 *            fraction of sqrt(996)
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 23, u64 n <= 51, u128 n <= 103,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Periodic part of the continued fraction of sqrt(996) */
static const unsigned short period[20] = {
    1, 1, 3, 1, 2, 2, 1, 1, 1, 4,
    1, 1, 1, 2, 2, 1, 3, 1, 1, 62
};
static const size_t PERIOD_LEN = 20;

/* a0 = floor(sqrt(996)) */
static const seq_t A0 = 31;

/* Numerators of convergents to sqrt(996) */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;    /* table of already computed numerators */
    static idx_t cache_len = 0;    /* size of the table */

    /* State of recurrence: p_{k-1} and p_{k-2} for the next pending k. */
    static seq_t p_minus1 = 1;    /* p_{-1}  */
    static seq_t p_minus2 = 0;    /* p_{-2}  */

    /* Return cached value if already available. */
    if (n < cache_len)
        return cache[n];

    /* First call: initialise cache and recurrence state. */
    if (cache == NULL) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) return 0;                /* allocation failed */
        cache[0] = A0;                       /* p_0  */
        cache_len = 1;
        /* After computing p_0, set recurrence state for k = 1. */
        p_minus2 = 1;   /* p_{-1} for the next step */
        p_minus1 = A0;  /* p_0  */
        if (n == 0)
            return cache[0];
    }

    /* Expand cache if we need more terms. */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;                /* allocation failed */
        cache = tmp;

        for (idx_t k = cache_len; k <= n; ++k) {
            seq_t ak = (k == 0) ? A0 : period[(k - 1) % PERIOD_LEN];
            seq_t pk = (seq_t)(ak * p_minus1 + p_minus2);
            cache[k] = pk;

            /* Update recurrence state for next k. */
            p_minus2 = p_minus1;
            p_minus1 = pk;
        }
        cache_len = new_len;
    }

    return cache[n];
}
