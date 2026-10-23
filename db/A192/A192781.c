/*
 * A192781  Coefficient of x in the reduction of the n-th Fibonacci polynomial
 *          by x^3->x^2+1.
 * https://oeis.org/A192781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x in the n-th Fibonacci polynomial after
 *            reducing x^3 → x^2 + 1. Computed using the recurrence on the
 *            reduced polynomial coefficients: a_n = a_{n-1} + b_{n-1} +
 *            a_{n-2} b_n = c_{n-1} + b_{n-2} c_n = a_{n-1} + c_{n-2} where
 *            a_n, b_n, c_n are the coefficients of x^2, x, 1 in t...
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Necessary for malloc / realloc */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of previous polynomial coefficients */
    static seq_t *a_arr = NULL;
    static seq_t *b_arr = NULL;
    static seq_t *c_arr = NULL;
    static idx_t cap = 0;          /* number of terms currently stored */

    if (n == 0)   /* undefined, but return 0 for safety */
        return 0;

    /* Extend cache if needed */
    if ((idx_t)n > cap)
    {
        idx_t new_cap = n;

        /* Reallocate coefficient arrays to the new size */
        seq_t *new_a = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
        seq_t *new_b = (seq_t *)realloc(b_arr, new_cap * sizeof(seq_t));
        seq_t *new_c = (seq_t *)realloc(c_arr, new_cap * sizeof(seq_t));

        /* If allocation failed, keep the old arrays (caller may handle error) */
        if (!new_a || !new_b || !new_c)
            return 0;                 /* fallback, should not happen */

        a_arr = new_a;  b_arr = new_b;  c_arr = new_c;

        /* Populate new terms */
        for (idx_t i = cap; i < new_cap; ++i)
        {
            if (i == 0) {                 /* F1(x) = 1  => a=0, b=0, c=1 */
                a_arr[i] = 0;
                b_arr[i] = 0;
                c_arr[i] = 1;
            }
            else if (i == 1) {             /* F2(x) = x  => a=0, b=1, c=0 */
                a_arr[i] = 0;
                b_arr[i] = 1;
                c_arr[i] = 0;
            }
            else {                         /* Recurrence for i ≥ 2 (n = i+1) */
                a_arr[i] = a_arr[i-1] + b_arr[i-1] + a_arr[i-2];
                b_arr[i] = c_arr[i-1] + b_arr[i-2];
                c_arr[i] = a_arr[i-1] + c_arr[i-2];
            }
        }
        cap = new_cap;
    }

    return b_arr[n-1];   /* coefficient of x in reduced F_n(x) */
}
