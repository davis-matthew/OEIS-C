/*
 * A111942  Column 0 of the matrix logarithm (A111941) of triangle A111940,
 *          which shifts columns left and up under matrix inverse; these terms
 *          are the result of multiplying the element in row n by n!.
 * https://oeis.org/A111942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = ((n-1)*(n-2)*a(n-2) - 2*a(n-1)) / 4, n > 2 Initial
 *            values: a(0)=0, a(1)=1, a(2)=-1 This recurrence yields the OEIS
 *            sequence A111942
 * Range:     i8 n <= 7, i16 n <= 10, i32 n <= 16, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 24 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Allocate an array of seq_t, compatible with C and C++ */
static inline seq_t *alloc_seq_array(size_t cap)
{
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t *)malloc(cap * sizeof(seq_t));
#endif
}

/* dynamic array helpers -------------------------------------------------- */
static seq_t *resize_array(seq_t *old, idx_t old_size, idx_t new_size)
{
    seq_t *new_arr;
#ifdef __cplusplus
    new_arr = new seq_t[new_size]();        /* value–init for C++ objects   */
#else
    new_arr = (seq_t *)calloc(new_size, sizeof(seq_t));   /* zero for C */
#endif
    /* copy old data */
    idx_t i;
    for (i = 0; i < old_size && i < new_size; ++i)
        new_arr[i] = old[i];

    /* free old storage */
    if (old) {
#ifdef __cplusplus
        delete[] old;
#else
        free(old);
#endif
    }
    return new_arr;
}
#include <stddef.h>

seq_t a(idx_t n) {
    static seq_t *tbl = NULL;   /* memoized values of a(k) */
    static idx_t tbl_size = 0;  /* number of entries currently stored */
    static idx_t tbl_cap  = 0;  /* current allocated capacity */

    /* initialise the table on first call or if size is too small */
    if (tbl == NULL) {
        /* allocate enough space for at least the first 3 terms or up to n */
        idx_t init_cap = (n >= 3 ? n + 1 : 3);
        tbl = alloc_seq_array(init_cap);
        /* set initial terms */
        if (init_cap >= 1) tbl[0] = (seq_t)0;
        if (init_cap >= 2) tbl[1] = (seq_t)1;
        if (init_cap >= 3) tbl[2] = - (seq_t)1;
        tbl_size = init_cap;
        tbl_cap  = init_cap;
    }

    /* if we already have the requested term, just return it */
    if (n < tbl_size) {
        return tbl[n];
    }

    /* grow the table if necessary */
    if (tbl_size <= n) {
        idx_t new_cap = tbl_cap;
        if (new_cap == 0) new_cap = 8;
        while (new_cap <= n) new_cap <<= 1;   /* double until enough */

        tbl = resize_array(tbl, tbl_cap, new_cap);
        tbl_cap = new_cap;
    }

    /* compute missing terms up to n using the recurrence */
    for (idx_t i = tbl_size; i <= n; ++i) {
        if (i == 0) {
            tbl[i] = (seq_t)0;
        } else if (i == 1) {
            tbl[i] = (seq_t)1;
        } else if (i == 2) {
            tbl[i] = - (seq_t)1;
        } else {
            /* a(i) = ((i-1)*(i-2)*a(i-2) - 2*a(i-1)) / 4 */
            seq_t term1 = (seq_t)(i - 1) * (seq_t)(i - 2);   /* (i-1)*(i-2) */
            term1 = term1 * tbl[i - 2];                     /* * a(i-2) */
            seq_t term2 = tbl[i - 1] * ((seq_t)2);           /* 2*a(i-1) */
            tbl[i] = (term1 - term2) / ((seq_t)4);          /* divide by 4 */
        }
    }

    tbl_size = n + 1;      /* we now have values up to index n inclusive */
    return tbl[n];
}
