/*
 * A128713  Expansion of q^(-3/8)* eta(q)^7* eta(q^4)^2/ eta(q^2)^3 in powers
 *          of q.
 * https://oeis.org/A128713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   Euler transform of period‑4 sequence [-7, -4, -7, -6, ...]. a(n)
 *            = (1/n) * Σ_{k=1..n} s(k) * a(n-k), a(0)=1, where s(k) = Σ_{d|k}
 *            d * p(d) and p(d) is periodic with p(1)=-7, p(2)=-4, p(3)=-7,
 *            p(4)=-6
 * Range:     i8 n <= 6, i16 n <= 67, i32 n <= 2500, i64 n <= 2500,
 *            i128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;      /* computed a(0..len-1) */
static idx_t cache_len = 0;      /* current size of cache (len) */

/* returns the period‑4 value p(n) = -7,-4,-7,-6,...  */
static inline int p_val(idx_t n)
{
    switch ((int)((n - 1) % 4)) {
        case 0: return -7;   /* n % 4 == 1 */
        case 1: return -4;   /* n % 4 == 2 */
        case 2: return -7;   /* n % 4 == 3 */
        case 3: return -6;   /* n % 4 == 0 */
        default: return 0;   /* never reached */
    }
}

seq_t a(idx_t n)
{
    /* extend cache if needed */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!new_cache) return 0; /* allocation failed */

        /* compute s(k) for k = 1..new_len-1 */
        seq_t *s = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!s) { free(new_cache); return 0; }
        for (idx_t i = 0; i < new_len; ++i) s[i] = 0;

        for (idx_t d = 1; d < new_len; ++d) {
            int pd = p_val(d);
            seq_t val = (seq_t)d * (seq_t)pd;     /* d * p(d) */
            for (idx_t m = d; m < new_len; m += d)
                s[m] += val;
        }

        /* compute the a sequence */
        new_cache[0] = (seq_t)1;
        for (idx_t cur = 1; cur < new_len; ++cur) {
            seq_t sum = 0;
            for (idx_t k = 1; k <= cur; ++k)
                sum += s[k] * new_cache[cur - k];
            new_cache[cur] = sum / (seq_t)cur;   /* exact division */
        }

        /* free old cache and temporary arrays, replace with new_cache */
        free(cache);
        free(s);
        cache = new_cache;
        cache_len = new_len;
    }
    return cache[n];
}
