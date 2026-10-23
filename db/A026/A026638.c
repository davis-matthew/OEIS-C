/*
 * A026638  a(n) = A026637(2*n, n).
 * https://oeis.org/A026638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A026637(2*n, n)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (for n ≥ 3): 2*n*a(n) = (7*n-4)*a(n‑1) + 2*(2*n-1)*a(n‑2). */
/* Initial values: a(0)=1, a(1)=2, a(2)=8. */

#include <stdlib.h>

/* Computes the n-th term of OEIS A026638. */
seq_t a(idx_t n)
{
    /* Static cache to memoise computed terms. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0; /* number of allocated slots */

    /* Increase cache size if needed. */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();               /* memory exhaustion */
        cache = tmp;

        /* Compute missing terms. */
        for (idx_t i = cache_size; i < new_size; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1) {
                cache[i] = (seq_t)2;
            } else if (i == 2) {
                cache[i] = (seq_t)8;
            } else {
                /* For n ≥ 3 use the recurrence. */
                if (sizeof(seq_t) <= 8) {   /* 8-byte (or smaller) types */
                    unsigned __int128 coeff1 = (unsigned __int128)7 * i - 4;
                    unsigned __int128 coeff2 = (unsigned __int128)2 *
                                               ((unsigned __int128)(2 * i) - 1);
                    unsigned __int128 denom  = (unsigned __int128)2 * i;

                    unsigned __int128 numer =
                        coeff1 * (unsigned __int128)cache[i-1] +
                        coeff2 * (unsigned __int128)cache[i-2];

                    cache[i] = (seq_t)(numer / denom);
                } else {                    /* arbitrary‑precision types */
                    /* Use seq_t for intermediate because the class handles big ints. */
                    seq_t coeff1 = (seq_t)(7 * i - 4);
                    seq_t coeff2 = (seq_t)(2 * ((seq_t)(2 * i) - 1));
                    seq_t denom  = (seq_t)(2 * i);

                    seq_t numer =
                        coeff1 * cache[i-1] + coeff2 * cache[i-2];

                    cache[i] = numer / denom;
                }
            }
        }
        cache_size = new_size;
    }

    return cache[n];
}
