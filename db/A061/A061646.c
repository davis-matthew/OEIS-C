/*
 * A061646  a(n) = 2*a(n-1) + 2*a(n-2) - a(n-3) with a(-1) = 1, a(0) = 1, a(1)
 *          = 1.
 * https://oeis.org/A061646
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - a(n-3) with a(-1) = 1, a(0) = 1,
 *            a(1) = 1
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h> /* malloc, realloc */

seq_t a(idx_t n)
{
    /* Only valid for n >= -1; below that behaviour is undefined. */
    if (n < -1)
        return (seq_t)0;  /* fallback */

    /* Map index n to array position: a(-1)->0, a(0)->1, a(1)->2, ... */
    size_t pos = (size_t)(n + 1);

    /* Static storage for computed values. */
    static seq_t *arr = NULL;
    static size_t len  = 0;   /* number of elements stored */
    static size_t cap  = 0;   /* allocated capacity */

    /* Initialize storage once with the first three terms. */
    if (len == 0) {
        cap = 3;
        arr = (seq_t *)malloc(cap * sizeof(seq_t));
        arr[0] = (seq_t)1; /* a(-1) */
        arr[1] = (seq_t)1; /* a(0)  */
        arr[2] = (seq_t)1; /* a(1)  */
        len = 3;
    }

    /* Grow array if needed. */
    if (pos >= len) {
        size_t new_cap = cap;
        while (pos >= new_cap)
            new_cap = new_cap ? new_cap * 2 : 8;  /* grow exponentially */

        if (new_cap > cap) {
            seq_t *tmp = (seq_t *)realloc(arr, new_cap * sizeof(seq_t));
            if (!tmp) {
                /* Allocation failed – return zero to avoid crash. */
                return (seq_t)0;
            }
            arr = tmp;
            cap = new_cap;
        }

        /* Compute missing sequence values up to 'pos'. */
        for (size_t i = len; i <= pos; ++i) {
            /* Use the recurrence: a(i) = 2*a(i-1) + 2*a(i-2) - a(i-3) */
            seq_t t1 = arr[i-1] + arr[i-1]; /* 2*a(i-1) */
            seq_t t2 = arr[i-2] + arr[i-2]; /* 2*a(i-2) */
            arr[i] = t1 + t2 - arr[i-3];
        }
        len = pos + 1;
    }

    return arr[pos];
}
