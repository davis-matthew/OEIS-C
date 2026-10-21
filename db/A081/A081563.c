/*
 * A081563  Second binomial transform of expansion of exp(2*cosh(x)).
 * https://oeis.org/A081563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(n+1)=2*a(n)+2*∑_{k=0}^{n-1} a(k)*C(n,k) if (n-k) is
 *            odd (n≥0)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 24, u128 n <= 41,
 *            big any n
 * Helpers:   xn_xk
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/xn_xk.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap    = 0;
    static idx_t  last   = (idx_t)-1;   /* no values computed yet */

    /* Grow cache if needed */
    if ((idx_t)cap <= n) {
        size_t new_cap = cap ? cap * 2 : 8;
        while ((idx_t)new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                 /* allocation failure */
        if (tmp != cache) {
            /* new space at end */
            memset(tmp + cap, 0, (new_cap - cap) * sizeof(seq_t));
            cache = tmp;
        }
        cap = new_cap;
    }

    /* Initialise a(0) if needed */
    if (last == (idx_t)-1) {
        cache[0] = 1;
        last = 0;
    }

    /* Compute values up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t sum = 0;
        seq_t bin = 1;          /* C(i-1,0) */
        for (idx_t k = 0; k < i; ++k) {
            if (((i - 1 - k) & 1) != 0) {   /* (i-1-k) odd => include */
                sum += cache[k] * bin;
            }
            /* Update bin for next k+1: bin = C(i-1,k+1) */
            bin = bin * ((i - 1) - k);
            bin = bin / (k + 1);
        }
        cache[i] = cache[i - 1] * 2 + sum * 2;
    }
    last = n;
    return cache[n];
}
