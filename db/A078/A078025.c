/*
 * A078025  Expansion of (1-x)/(1-2*x^2+2*x^3).
 * https://oeis.org/A078025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 9, i16 n <= 18, i32 n <= 38, i64 n <= 77, i128 n <= 155,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=-1, a(2)=2; for n>=3, a(n)=2*a(n-2)-2*a(n-3) */
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *arr = NULL;
    static size_t cap = 0;
    static size_t len = 0;

    /* Initialize first three terms */
    if (!len) {
        cap = 4;   /* at least 4 to hold 3 terms + space for growth */
        arr = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!arr) exit(1);
        arr[0] = (seq_t)1;
        arr[1] = (seq_t)-1;
        arr[2] = (seq_t)2;
        len = 3;
    }

    /* Return cached value if already computed */
    if ((size_t)n < len)
        return arr[n];

    /* Grow storage if necessary */
    size_t need = (size_t)n + 1;
    if (need > cap) {
        while (cap < need) cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(arr, cap * sizeof(seq_t));
        if (!tmp) exit(1);
        arr = tmp;
    }

    /* Fill missing terms using the recurrence */
    for (size_t i = len; i <= (size_t)n; ++i) {
        arr[i] = (seq_t)2 * arr[i - 2] - (seq_t)2 * arr[i - 3];
    }
    len = need;
    return arr[n];
}
