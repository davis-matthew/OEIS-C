/*
 * A107710  Numbers of the form (6^i)*(13^j), with i, j >= 0.
 * https://oeis.org/A107710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 64, u64 n <= 236, u128 n <= 900,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Code to generate sequence A107710: numbers of the form 6^i * 13^j (i,j ≥ 0), in increasing order */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static state for memoisation and incremental generation */
    static seq_t *vals  = NULL;   /* array of generated terms          */
    static idx_t  sz    = 0;       /* number of terms stored            */
    static idx_t  cap   = 0;       /* allocated capacity of vals        */

    /* indices for generating next multiples of 6 and 13 */
    static idx_t  i6    = 0;
    static idx_t  i13   = 0;
    static seq_t  next6 = 0;
    static seq_t  next13 = 0;

    if (n == 0)
        return 0;                 /* offset starts at 1, a(0) is undefined */

    /* Ensure capacity for at least n terms */
    if (sz < n) {
        if (sz == 0) {
            /* First allocation: start with capacity 1 */
            cap = 1;
        } else {
            /* Grow while capacity < n */
            while (cap < n)
                cap <<= 1;
        }

        seq_t *new_vals = NULL;
#ifdef __cplusplus
        new_vals = new seq_t[cap];
#else
        new_vals = (seq_t *)malloc(cap * sizeof(seq_t));
#endif
        if (!new_vals)
            return 0;  /* allocation failure */

        /* Copy existing terms to new storage */
        for (idx_t i = 0; i < sz; ++i)
            new_vals[i] = vals[i];

        /* Free old storage */
#ifdef __cplusplus
        delete [] vals;
#else
        free(vals);
#endif
        vals = new_vals;
    }

    /* Initialise first term if it hasn't been set yet */
    if (sz == 0) {
        vals[0] = (seq_t)1;   /* 6^0 * 13^0 */
        sz = 1;
        /* initialise indices and next multiples */
        i6 = 0;
        i13 = 0;
        next6 = vals[i6] * (seq_t)6;
        next13 = vals[i13] * (seq_t)13;
    }

    /* If the requested term is already computed, return it */
    if (sz >= n)
        return vals[n - 1];

    /* Generate terms until we reach the requested index */
    while (sz < n) {
        seq_t next_val = (next6 < next13) ? next6 : next13;
        vals[sz++] = next_val;

        if (next_val == next6) {
            ++i6;
            next6 = vals[i6] * (seq_t)6;
        }
        if (next_val == next13) {
            ++i13;
            next13 = vals[i13] * (seq_t)13;
        }
    }

    return vals[n - 1];
}
