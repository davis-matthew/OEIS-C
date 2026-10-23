/*
 * A169986  Ceiling(phi^n) where phi = (1+sqrt(5))/2.
 * https://oeis.org/A169986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) - a(n-3) - a(n-4) (n ≥ 5) * Initial
 *            values: a(0)=1, a(1)=2, a(2)=3, a(3)=5, a(4)=7 * * This
 *            implementation builds the sequence on demand, caching all *
 *            computed values. It works for any idx_t n (up to the limits of
 *            the * machine), and for any seq_t type supported by the b...
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

static seq_t *a_arr = NULL;   /* cache of computed terms */
static size_t a_len = 0;      /* number of terms currently cached */

/* Ensure that a_arr has values up to index n. */
static void ensure_a(idx_t n) {
    /* Convert idx_t to size_t safely for the loop bounds. */
    size_t required = (size_t)n + 1;
    if (required <= a_len) return; /* already computed */

    /* Reallocate the cache as needed. */
    seq_t *tmp;
    if (a_arr == NULL) {
        tmp = (seq_t *)malloc(required * sizeof(seq_t));
    } else {
        tmp = (seq_t *)realloc(a_arr, required * sizeof(seq_t));
    }
    if (tmp == NULL) return;           /* allocation failed – value will be garbage */
    a_arr = tmp;

    /* Compute any missing values. */
    for (size_t i = a_len; i < required; ++i) {
        if (i == 0) {
            a_arr[i] = 1;
        } else if (i == 1) {
            a_arr[i] = 2;
        } else if (i == 2) {
            a_arr[i] = 3;
        } else if (i == 3) {
            a_arr[i] = 5;
        } else if (i == 4) {
            a_arr[i] = 7;
        } else {
            /* recurrence: a(n) = a(n-1)+2*a(n-2)-a(n-3)-a(n-4) */
            a_arr[i] = a_arr[i-1] + 2 * a_arr[i-2] - a_arr[i-3] - a_arr[i-4];
        }
    }

    a_len = required;
}

seq_t a(idx_t n) {
    ensure_a(n);
    return a_arr[(size_t)n];
}
