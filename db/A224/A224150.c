/*
 * A224150  Number of 6 X n 0..1 arrays with rows and antidiagonals unimodal
 *          and columns nondecreasing.
 * https://oeis.org/A224150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 30, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (recurrence): 
   a(1)=7, a(2)=49, a(3)=242, a(4)=930, a(5)=2985, a(6)=8375, a(7)=21183,
   a(8)=49365, a(9)=107697, a(10)=222603, a(11)=439909, a(12)=837071,
   a(13)=1542126;
   For n>13: 
   a(n)=13*a(n-1) - 78*a(n-2) + 286*a(n-3) - 715*a(n-4) + 1287*a(n-5)
        - 1716*a(n-6) + 1716*a(n-7) - 1287*a(n-8) + 715*a(n-9)
        - 286*a(n-10) + 78*a(n-11) - 13*a(n-12) + a(n-13)
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* initial 13 terms, indexed from 0 to 12 corresponding to n=1..13 */
    static const seq_t init[13] = {
        (seq_t)7,      /* a(1) */
        (seq_t)49,     /* a(2) */
        (seq_t)242,    /* a(3) */
        (seq_t)930,    /* a(4) */
        (seq_t)2985,   /* a(5) */
        (seq_t)8375,   /* a(6) */
        (seq_t)21183,  /* a(7) */
        (seq_t)49365,  /* a(8) */
        (seq_t)107697, /* a(9) */
        (seq_t)222603, /* a(10) */
        (seq_t)439909, /* a(11) */
        (seq_t)837071, /* a(12) */
        (seq_t)1542126 /* a(13) */
    };

    if (n < 1) return (seq_t)0;          /* out of offset range */
    if (n <= 13u) return init[n - 1];    /* direct return, 1‑based index */

    /* allocate array of size n+1, index 0 unused */
    seq_t *arr = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!arr) return (seq_t)0;           /* allocation failed */

    /* copy initial values */
    for (size_t i = 1; i <= 13u; ++i)
        arr[i] = init[i - 1];

    /* coefficients for the recurrence, from a(n-1) to a(n-13) */
    const int coeffs[13] = {
        13, -78, 286, -715, 1287, -1716,
        1716, -1287, 715, -286, 78, -13, 1
    };

    /* compute terms from 14 to n */
    for (size_t i = 14u; i <= n; ++i) {
        seq_t pos = (seq_t)0; /* sum of positive contributions */
        seq_t neg = (seq_t)0; /* sum of absolute values of negative contributions */
        for (int k = 1; k <= 13; ++k) {
            int c = coeffs[k - 1];
            long long mult = (c > 0) ? (long long)c : -(long long)c;   /* |c| */
            seq_t prod = arr[i - k] * (seq_t)mult;
            if (c > 0)
                pos = pos + prod;
            else
                neg = neg + prod;
        }
        arr[i] = pos - neg; /* guaranteed non‑negative */
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
