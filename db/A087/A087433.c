/*
 * A087433  Expansion of g.f.: (1-2*x)*(1-4*x+x^2)/((1-x)*(1-3*x)*(1-4*x)).
 * https://oeis.org/A087433
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8*a(n-1) - 19*a(n-2) + 12*a(n-3) with initial values
 *            a(0)=1, a(1)=2, a(2)=6, a(3)=20. The function memoises computed
 *            terms; no recursion is used
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[4] = { (seq_t)1, (seq_t)2, (seq_t)6, (seq_t)20 };
static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_size = 4;                          /* store initial 4 terms */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) exit(1);                     /* allocation failure */
        for (idx_t i = 0; i < 4; ++i) cache[i] = init_vals[i];
    }

    if (n < cache_size)
        return cache[n];                         /* already known term */

    /* Extend cache up to required index */
    if (n >= cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) exit(1);                       /* allocation failure */
        cache = tmp;

        for (idx_t i = cache_size; i <= n; ++i) {
            cache[i] = cache[i-1] * (seq_t)8
                       - cache[i-2] * (seq_t)19
                       + cache[i-3] * (seq_t)12;
        }
        cache_size = n + 1;
    }

    return cache[n];
}
