/*
 * A041660  Numerators of continued fraction convergents to sqrt(349).
 * https://oeis.org/A041660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(0)=18, a(1)=19, a(n)=p(n-1)*a(n-1)+a(n-2) (n≥2), where p(k)
 *            are the partial quotients of sqrt(349): period = {1, 2, 7, 7, 2,
 *            1, 36}
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cached convergent numerators */
static idx_t  cache_len = 0;      /* number of cached terms */

static const int period[7] = {1, 2, 7, 7, 2, 1, 36};

seq_t a(idx_t n)
{
    /* Ensure base terms a(0)=18, a(1)=19 are available */
    if (cache_len == 0) {
        cache_len = 2;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;           /* allocation failure – return 0 */
        cache[0] = 18;                   /* p0 */
        cache[1] = 19;                   /* p1 */
    }

    if ((idx_t)cache_len <= n) {
        /* grow cache */
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;              /* allocation failure – return 0 */
        cache = tmp;

        /* compute missing terms */
        idx_t old_len = cache_len; /* last pre‑computed index +1 */
        seq_t prev2 = cache[old_len - 2]; /* a(old_len-2) */
        seq_t prev1 = cache[old_len - 1]; /* a(old_len-1) */

        for (idx_t i = old_len; i <= n; ++i) {
            int coef = period[(i - 1) % 7];          /* a_i for i>=1 */
            seq_t val = (seq_t)coef * prev1 + prev2; /* recurrence */
            cache[i] = val;
            prev2 = prev1;
            prev1 = val;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
