/*
 * A185221  E.g.f. is solution to y = 1 + log(1 + x*y) in powers of x.
 * https://oeis.org/A185221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! * Sum_{k=0..n} Stirling1(n,k)/(n-k+1)! (Stirling1:
 *            signed Stirling number of the first kind)
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 19, i128 n <= 32,
 *            big any n
 * Helpers:   alloc_seq_array_custom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/alloc_seq_array_custom.h"

#ifdef __cplusplus
namespace { }
#endif

static inline void free_seq_array_custom(seq_t* arr) {
#ifdef __cplusplus
    delete[] arr;
#else
    free(arr);
#endif
}

/* Ensure that *arr has capacity at least needed+1.
   The function copies existing elements and frees the old array. */
static void ensure_capacity(seq_t **arr, size_t *cap, size_t needed) {
    if (*cap <= needed) {
        size_t new_cap = (*cap) ? (*cap) : 1;
        while (new_cap <= needed) new_cap *= 2;
        seq_t *tmp = alloc_seq_array_custom(new_cap);
        if (*arr) {
#ifdef __cplusplus
            for (size_t i = 0; i < *cap; ++i) tmp[i] = (*arr)[i];
#else
            memcpy(tmp, *arr, (*cap) * sizeof(seq_t));
#endif
            free_seq_array_custom(*arr);
        }
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Compute the OEIS A185221 term a(n)   */
seq_t a(idx_t n) {
    /* static state: all arrays are kept here to be shared between calls */
    static seq_t *a_cache = NULL;      /* a values */
    static size_t a_cap   = 0;
    static seq_t *fac = NULL;          /* factorials */
    static size_t fac_cap   = 0;
    static seq_t *s = NULL;            /* current row of signed Stirling numbers */
    static size_t s_cap   = 0;
    static idx_t  max_n    = 0;        /* highest n for which a_cache[0..max_n] are valid */

    /* Initialisation for n = 0 */
    if (max_n == 0 && a_cache == NULL) {
        ensure_capacity(&a_cache, &a_cap, 0);
        a_cache[0] = (seq_t)1;

        ensure_capacity(&fac, &fac_cap, 0);
        fac[0] = (seq_t)1;

        ensure_capacity(&s, &s_cap, 0);
        s[0] = (seq_t)1;
    }

    /* If we have not yet computed up to n, extend the tables */
    if (n > max_n) {
        for (idx_t i = max_n + 1; i <= n; ++i) {
            /* Ensure enough capacity for new index i */
            ensure_capacity(&fac, &fac_cap, i);
            ensure_capacity(&s,   &s_cap,   i);
            ensure_capacity(&a_cache, &a_cap, i);

            /* Compute factorial: fac[i] = fac[i-1] * i */
            fac[i] = fac[i-1] * (seq_t)i;

            /* Update Stirling row to n = i */
            s[i] = (seq_t)0;                  /* value for k = i (was 0 in previous row) */
            for (idx_t k = i; k >= 1; --k) {
                seq_t prev = s[k-1];           /* s(n-1, k-1)  */
                s[k] = prev - (seq_t)(i-1) * s[k]; /* s(n-1, k)   */
            }
            s[0] = (seq_t)0;                  /* s(n,0)=0 for n>0 */

            /* Compute a[i] = sum_{k=1..i} s[i][k] * (i!/(i-k+1)!)   */
            seq_t sum   = (seq_t)0;
            seq_t ratio = (seq_t)1;           /* ratio = i!/(i-k+1)!   */

            for (idx_t k = 1; k <= i; ++k) {
                if (k > 1)                       /* update ratio multiplicatively */
                    ratio = ratio * (seq_t)(i - k + 2);

                sum += s[k] * ratio;
            }
            a_cache[i] = sum;
        }
        max_n = n;
    }

    return a_cache[n];
}
