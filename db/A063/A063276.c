/*
 * A063276  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          3 ).
 * https://oeis.org/A063276
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) satisfies the linear recurrence a(n) = a(n-1) - a(n-3) +
 *            2*a(n-4) - a(n-5) + a(n-7) - a(n-8) for n ≥ 12. The first twelve
 *            values (n = 2 … 12) are a(2) = -1, a(3) = 0, a(4) = 0, a(5) = 0,
 *            a(6) = 0, a(7) = 1, a(8) = 1, a(9) = 1, a(10) = 2, a(11) = 2,
 *            a(12) = 2
 * Range:     i8 n <= 78, i16 n <= 78, i32 n <= 78, i64 n <= 78, i128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;                      /* undefined for n < 2 */

    /* Static cache: arr[i] == a(i+2), i = 0 … */
    static seq_t *arr = NULL;
    static size_t arr_len = 0;                  /* number of entries kept */
    static idx_t  max_n = 1;                    /* highest n computed so far */

    /* initialise cache with the first twelve terms */
    if (!arr) {
        arr_len = 12;                           /* indices 0 … 11 */
        arr = (seq_t *)malloc(arr_len * sizeof(seq_t));
        if (!arr)
            return (seq_t)0;                    /* allocation failure */

        arr[0] = (seq_t)-1; /* a(2) */
        arr[1] = (seq_t)0;  /* a(3) */
        arr[2] = (seq_t)0;  /* a(4) */
        arr[3] = (seq_t)0;  /* a(5) */
        arr[4] = (seq_t)0;  /* a(6) */
        arr[5] = (seq_t)1;  /* a(7) */
        arr[6] = (seq_t)1;  /* a(8) */
        arr[7] = (seq_t)1;  /* a(9) */
        arr[8] = (seq_t)2;  /* a(10) */
        arr[9] = (seq_t)2;  /* a(11) */
        arr[10] = (seq_t)2; /* a(12) */
        /* arr[11] is unused yet */

        max_n = 12;
    }

    /* grow cache if necessary */
    if ((size_t)(n - 1) > arr_len) {
        size_t new_len = arr_len;
        while ((size_t)(n - 1) > new_len)
            new_len *= 2;
        seq_t *tmp = (seq_t *)realloc(arr, new_len * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;                    /* allocation failure */
        arr = tmp;
        arr_len = new_len;
    }

    /* compute missing terms, starting from max_n+1 up to n */
    for (idx_t m = max_n + 1; m <= n; ++m) {
        if (m <= 12)                  /* already known */
            continue;

        /* indices for recurrence:
           arr[j] holds a(j+2)
           indices: a(m-1) → arr[m-3]
                    a(m-3) → arr[m-5]
                    a(m-4) → arr[m-6]
                    a(m-5) → arr[m-7]
                    a(m-7) → arr[m-9]
                    a(m-8) → arr[m-10]
        */
        seq_t val = arr[m - 3];           /* a(m-1) */
        val -= arr[m - 5];                /* - a(m-3) */
        val += (seq_t)2 * arr[m - 6];     /* + 2*a(m-4) */
        val -= arr[m - 7];                /* - a(m-5) */
        val += arr[m - 9];                /* + a(m-7) */
        val -= arr[m -10];                /* - a(m-8) */

        arr[m - 2] = val;                  /* store a(m) */
    }

    if (n > max_n)
        max_n = n;

    return arr[n - 2];
}
