/*
 * A132436  A binomial recursion: a(n) = p(n) (see comment).
 * https://oeis.org/A132436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n>1, a(n) = Σ_{k=1}^{n-1} (binom(n,k) – 1) * a(k)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the recurrence derived from z(n) = p(n)*x + q(n)
   with z(1) = x and z(n) = 1 + Σ_{k=1}^{n-1} (-1 + binom(n,k))*z(k). */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache[1..max_n] holds computed a(n) values */
static size_t cache_size = 0; /* highest n for which cache[n] is valid */

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for n < 1, return 0 */

    size_t need = (size_t)n;
    if (need > cache_size) {             /* extend cache up to n */
        size_t new_size = need + 1;      /* include index 0 which is unused */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);    /* abort on allocation failure */
        cache = tmp;

        for (size_t m = cache_size + 1; m <= need; ++m) {
            if (m == 1) {                /* base case */
                cache[1] = (seq_t)1;
                continue;
            }

            seq_t sum = (seq_t)0;
            seq_t bin = (seq_t)m;        /* C(m,1) = m */

            for (size_t k = 1; k < m; ++k) {
                seq_t term = bin - (seq_t)1;          /* binom(m,k) - 1 */
                sum += term * cache[k];               /* accumulate product */

                /* update bin to C(m, k+1) */
                seq_t num = (seq_t)(m - k);
                seq_t den = (seq_t)(k + 1);
                bin = (bin * num) / den;
            }

            cache[m] = sum;             /* store a(m) */
        }
        cache_size = need;
    }

    return cache[n];
}
