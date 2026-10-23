/*
 * A107885  Column 0 of triangle A107884.
 * https://oeis.org/A107885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{n-1} a(k) * binom(3 + k(k-1)/2, n-k) *
 *            (-1)^(n-k+1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   binom_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This recurrence follows from 1 = Σ_{k≥0} a(k)·x^k·(1-x)^(3 + k(k-1)/2).  */
#include <stdlib.h>
#include "../utils/binom_small.h"

seq_t a(idx_t n)
{
    /* cached values of a(k) */
    static seq_t *array = NULL;
    static idx_t size  = 0;   /* number of cached entries */
    static idx_t cap   = 0;   /* allocated capacity */

    /* grow the cache to cover index n */
    while (size <= n) {
        seq_t val;

        if (size == 0) {
            /* a(0) = 1 */
            val = (seq_t)1;
        } else {
            seq_t pos = 0;    /* sum of terms with sign +1 */
            seq_t neg = 0;    /* sum of terms with sign -1 */

            /* compute a(size) from a(0)…a(size-1) */
            for (idx_t kk = 0; kk < size; ++kk) {
                idx_t d   = size - kk;         /* n - k */
                idx_t E   = 3 + kk * (kk - 1) / 2; /* exponent in (1-x)^E */

                if (d > E)          /* term is zero */
                    continue;

                seq_t term = array[kk] * binom_small(E, d);

                /* sign = (-1)^(d+1) : + if d odd, - if d even */
                if ((d & 1) == 0)   /* d even → sign –1 */
                    neg += term;
                else               /* d odd → sign +1 */
                    pos += term;
            }
            /* a(n) = pos – neg  (pos ≥ neg for all n) */
            val = pos;
            val -= neg;
        }

        /* ensure capacity in the array */
        if (size + 1 > cap) {
            idx_t new_cap = (cap == 0) ? 4 : cap * 2;
            if (new_cap < size + 1)
                new_cap = size + 1;
            seq_t *tmp = (seq_t *)realloc(array, new_cap * sizeof(seq_t));
            if (!tmp)
                abort();            /* insufficient memory */
            array = tmp;
            cap = new_cap;
        }

        array[size] = val;
        ++size;
    }

    return array[n];
}
