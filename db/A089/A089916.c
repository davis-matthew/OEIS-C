/*
 * A089916  a(n) = 5^n *n! *L_n^{-1/5}(-1), where L_n^(alpha)(x) are
 *          generalized Laguerre polynomials.
 * https://oeis.org/A089916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (10*n -1)*a(n-1) - (n-1)*(25*n -30)*a(n-2) with a(0)=1,
 *            a(1)=9
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    /* static cache and state */
    static seq_t *cache = NULL;
    static size_t cap = 0;                /* allocated size */
    static idx_t computed_max = (idx_t)-1; /* highest n computed */

    /* helper: ensure cache has room for n+1 entries */
    if (cache == NULL) {
        size_t init = n + 1 > 8 ? (n + 1) * 2 : 8;
        cache = (seq_t*)malloc(init * sizeof(seq_t));
        if (cache == NULL) abort();
        memset(cache, 0, init * sizeof(seq_t));
        cap = init;
    } else if ((size_t)n + 1 > cap) {
        size_t new_cap = (n + 1) * 2;
        if (new_cap <= cap) new_cap = cap * 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        memset(tmp + cap, 0, (new_cap - cap) * sizeof(seq_t));
        cache = tmp;
        cap = new_cap;
    }

    /* initialize base values if not yet done */
    if (computed_max == (idx_t)-1) {
        cache[0] = (seq_t)1;
        if (n >= 1) cache[1] = (seq_t)9;
        computed_max = (n >= 1 ? 1 : 0);
    }

    /* compute missing terms up to n */
    if (n > computed_max) {
        for (idx_t k = computed_max + 1; k <= n; ++k) {
            if (k == 0) {
                cache[0] = (seq_t)1;
            } else if (k == 1) {
                cache[1] = (seq_t)9;
            } else {
                seq_t prev1 = cache[k-1];
                seq_t prev2 = cache[k-2];
                seq_t term1 = ((seq_t)((idx_t)10 * k - 1)) * prev1;
                seq_t term2 = ((seq_t)(k-1)) * ((seq_t)((idx_t)25 * k - 30)) * prev2;
                cache[k] = term1 - term2;
            }
        }
        computed_max = n;
    }

    return cache[n];
}
