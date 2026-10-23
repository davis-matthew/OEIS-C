/*
 * A193815  Triangular array: the fusion of polynomial sequences P and Q given
 *          by p(n,x) = x^n + x^(n-1) + ... + x+1 and q(n,x)=(x+1)^n.
 * https://oeis.org/A193815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by flattening the triangular array T(n,k)
 *            defined by T(n,k) = binomial(n+1,k) for 0 ≤ k ≤ n-1 T(n,n) = n
 *            So the sequence lists the entries of each row of this triangle
 *            in order: 1; 1,1; 1,3,2; 1,4,6,3; … This implementation builds
 *            the values on demand and caches them for efficien...
 * Range:     u8 n <= 31, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"   /* Provides: seq_t binomial(idx_t n, idx_t k); */

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache  = NULL;   /* pointer to cached values */
    static idx_t  cap    = 0;      /* allocated capacity of cache */
    static idx_t  len    = 0;      /* number of values stored */

    /* static counter for next row to compute */
    static idx_t next_row = 0;     /* 0-based row index */

    /* ensure cache has enough room to store additional terms */
    if (cap == 0) {
        cap = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
    }

    /* generate rows until we reach the requested index */
    while (len <= n) {
        idx_t row = next_row;          /* current row number */
        /* ensure the cache buffer can hold the entire row */
        while (cap < len + (row + 1)) {
            cap <<= 1;
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * cap);
        }

        /* compute all entries of the current row */
        for (idx_t k = 0; k <= row; ++k) {
            seq_t val;
            if (row == 0) {            /* special case for the first row */
                val = (seq_t)1;
            } else if (k == row) {     /* last element in the row */
                val = (seq_t)row;
            } else {                   /* binomial coefficient */
                val = binomial(row + 1, k);
            }
            cache[len++] = val;
        }
        ++next_row; /* move to the next row */
    }

    return cache[n];
}
