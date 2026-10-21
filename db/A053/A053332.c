/*
 * A053332  a(n) contains n digits (either '4' or '7') and is divisible by
 *          2^n.
 * https://oeis.org/A053332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = a(n-1) + 10^(n-1)*(4 + 3 * ((a(n-1) / 2^(n-1)) mod 2))
 *            with a(1) = 4. The function caches previously computed terms to
 *            allow arbitrary order of calls
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

static seq_t *terms = NULL;      /* cached terms, indices 0..n-1 */
static idx_t  cache_len = 0;     /* number of cached terms (a(1) -> terms[0]) */
static seq_t  pow10_next = 1;    /* 10^cache_len, used for next term */

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined, but safe fallback */

    /* extend cache up to n */
    if (n > cache_len) {
        /* reallocate buffer */
        seq_t *tmp = (seq_t*)realloc(terms, n * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failed */
        terms = tmp;

        for (idx_t idx = cache_len; idx < n; ++idx) {
            /* idx is zero‑based: term a(idx+1) */
            if (idx == 0) {
                /* a(1) = 4 */
                terms[0] = 4;
                pow10_next *= 10; /* prepare 10^1 for next step */
            } else {
                seq_t a_prev = terms[idx-1];      /* a(idx)   */
                seq_t shifted = a_prev >> idx;     /* a(idx) / 2^(idx)   */
                seq_t digit   = 4 + 3 * (shifted & 1);
                seq_t a_new   = digit * pow10_next + a_prev;
                terms[idx] = a_new;
                pow10_next *= 10;                  /* next power of 10   */
            }
        }
        cache_len = n;
    }

    return terms[n-1];
}
