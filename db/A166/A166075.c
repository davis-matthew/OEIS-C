/*
 * A166075  Number of reduced words of length n in Coxeter group on 31
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^10 = I.
 * https://oeis.org/A166075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 29*(a(n-1)+a(n-2)+...+a(n-9)) - 435*a(n-10) + c(n),
 *            a(k<0) = 0. The addend c(n) is the numerator of the generating
 *            function: c(0)=1, c(1…9)=2, c(10)=1, otherwise c(n)=0. This
 *            recurrence follows from the rational generating function
 *            (t^10+2t^9+…+t)/(1-29t-…-29t^9+435t^10)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/seq_alloc_array.h"

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* cache of computed values          */
    static idx_t cap     = 0;       /* allocated array capacity          */
    static idx_t used    = 0;       /* number of terms currently stored   */

    /* enlarge cache if needed */
    if (n >= cap) {
        idx_t new_cap = (cap == 0) ? 1 : cap << 1;
        while (new_cap <= n) new_cap <<= 1;

        seq_t *tmp = seq_alloc_array(new_cap);
        if (!tmp) exit(1);               /* out of memory */

        if (cache) {
            memcpy(tmp, cache, used * sizeof(seq_t));
            free(cache);
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* compute terms up to n */
    for (idx_t i = used; i <= n; ++i) {
        /* compute c(i) */
        seq_t cval;
        if      (i == 0)      cval = (seq_t)1;
        else if (i <= 9)      cval = (seq_t)2;
        else if (i == 10)     cval = (seq_t)1;
        else                  cval = (seq_t)0;

        /* sum of the 9 previous terms (a(i-1)…a(i-9)) */
        seq_t sum9 = 0;
        for (idx_t k = 1; k <= 9; ++k) {
            if (i >= k) sum9 += cache[i - k];
        }

        seq_t term1 = (seq_t)29 * sum9;          /* 29 * sum9          */
        seq_t term10 = (i >= 10) ? (seq_t)435 * cache[i - 10] : (seq_t)0; /* 435*a(i-10) */

        cache[i] = term1 - term10 + cval;
    }

    used = n + 1;
    return cache[n];
}
