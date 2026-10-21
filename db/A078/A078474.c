/*
 * A078474  a(1)=a(2)=a(3)=1, a(n)=n-a(a(n-1))-a(a(n-2))-a(a(n-3)).
 * https://oeis.org/A078474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*************************************************************
 * Formula:  a(1) = a(2) = a(3) = 1;
 *           a(n) = n - a(a(n-1)) - a(a(n-2)) - a(a(n-3))   (n ≥ 4)
 *************************************************************/

#include <stdlib.h>   /* malloc, realloc */

/* Static storage for memoised values */
static seq_t *a_arr   = NULL;   /* 1‑indexed array of computed terms */
static idx_t  arr_cap = 0;     /* current allocated capacity (unused 0 index) */
static idx_t  computed= 0;     /* highest index that has been initialized */

/* Ensure the array has room for indices up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if ((idx_t)n + 1 > arr_cap) {       /* arrays are 1‑based */
        idx_t new_cap = arr_cap ? arr_cap * 2 : 4;
        while ((idx_t)n + 1 > new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
        a_arr = tmp;
        arr_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)            /* undefined for n ≤ 0 */
        return (seq_t)0;

    /* first call – initialise array and first three terms */
    if (a_arr == NULL) {
        arr_cap = 4;                     /* 0..3 will exist */
        a_arr = (seq_t *)malloc(arr_cap * sizeof(seq_t));
        a_arr[0] = (seq_t)0;             /* unused */
        a_arr[1] = a_arr[2] = a_arr[3] = (seq_t)1;
        computed = 3;
    }

    /* compute any missing values up to n */
    if ((idx_t)n > computed) {
        ensure_capacity(n);
        for (idx_t i = computed + 1; i <= n; ++i) {
            seq_t val = (seq_t)i;              /* current index as seq_t */

            idx_t idx1 = (idx_t)a_arr[i-1];
            idx_t idx2 = (idx_t)a_arr[i-2];
            idx_t idx3 = (idx_t)a_arr[i-3];

            /* a(i) = i - a(a(i-1)) - a(a(i-2)) - a(a(i-3)) */
            val = val - a_arr[idx1] - a_arr[idx2] - a_arr[idx3];
            a_arr[i] = val;
        }
        computed = (idx_t)n;
    }

    return a_arr[n];
}
