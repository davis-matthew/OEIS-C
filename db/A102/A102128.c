/*
 * A102128  a(1) = 1; a(n) = sum of previous terms which divide n.
 * https://oeis.org/A102128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; For n > 1: a(n) = sum_{k=1}^{n-1} a(k) if a(k) divides
 *            n (modulo test) This implementation memoises computed values; it
 *            expands the array on demand and recomputes missing terms in
 *            O(n²) time, which is sufficient for n up to about 10 000
 * Range:     u8 n <= 93, u16 n <= 12317, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 97 terms through n = 8752 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use the provided array‑resizing helper */
#include <stdlib.h>

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

seq_t a(idx_t n)
{
    /* Memoisation array: a_arr[0] = a(1), a_arr[1] = a(2), ... */
    static seq_t *a_arr = NULL;   /* pointer to the array */
    static idx_t size = 0;        /* current length of a_arr (>=1 after init) */

    if (n == 0) return (seq_t)0;  /* undefined for n<1, return 0 */

    /* Initialise on first call if necessary */
    if (!a_arr) {
        a_arr = resize_array(NULL, 0, 1);  /* allocate space for a(1) */
        size = 1;
        a_arr[0] = (seq_t)1;              /* a(1) = 1 */
    }

    /* If the requested term is already computed, return it */
    if (n <= size)
        return a_arr[n-1];

    /* Expand the array to length n (if needed) */
    idx_t old_size = size;
    if (size < n) {
        a_arr = resize_array(a_arr, size, n);
        size = n;
    }

    /* Compute missing terms from old_size+1 up to n */
    for (idx_t m = old_size + 1; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        seq_t m_seq = (seq_t)m;            /* need seq_t for modulo */
        for (idx_t k = 1; k < m; ++k) {
            seq_t ak = a_arr[k-1];
            if ((m_seq % ak) == 0)
                sum += ak;
        }
        a_arr[m-1] = sum;
    }

    return a_arr[n-1];
}
