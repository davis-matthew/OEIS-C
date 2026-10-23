/*
 * A164685  Number of reduced words of length n in Coxeter group on 41
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^7 = I.
 * https://oeis.org/A164685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Recurrence derived from the rational generating function
   G(t) = (1 + 2t + 2t² + 2t³ + 2t⁴ + 2t⁵ + 2t⁶ + t⁷) /
          (1 - 39t - 39t² - 39t³ - 39t⁴ - 39t⁵ - 39t⁶ + 780t⁷)

   For n ≥ 8:
      a(n) = 39 * (a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)+a(n-6))
             - 780 * a(n-7)

   For n = 7:
      a(7) = 39 * (a(6)+a(5)+a(4)+a(3)+a(2)+a(1))
             - 780 * a(0) + 1

   Base values:
      a(0)=1, a(1)=41, a(2)=1640, a(3)=65600,
      a(4)=2624000, a(5)=104960000, a(6)=4198400000
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *arr = NULL;
    static size_t cap = 0;          /* allocated capacity (number of elements) */
    static idx_t last = 0;          /* largest index computed so far */

    /* Initialise the cache lazily on first call. */
    if (!arr) {
        cap = 64;                    /* start with some reasonable space */
        arr = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!arr) abort();           /* out of memory */

        /* Base terms */
        arr[0] = (seq_t)1;
        arr[1] = (seq_t)41;
        arr[2] = (seq_t)1640;
        arr[3] = (seq_t)65600;
        arr[4] = (seq_t)2624000;
        arr[5] = (seq_t)104960000;
        arr[6] = (seq_t)4198400000;
        last   = 6;
    }

    if (n <= last)
        return arr[n];

    /* Grow the array if necessary. */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = cap;
        while (new_cap <= n)
            new_cap <<= 1;          /* double until big enough */
        arr = (seq_t *)realloc(arr, new_cap * sizeof(seq_t));
        if (!arr) abort();          /* out of memory */
        cap = new_cap;
    }

    /* Compute all missing terms up to n using the recurrence. */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t sum = 0;
        /* Sum the previous six terms a(i-1)..a(i-6) */
        for (idx_t j = 1; j <= 6; ++j)
            sum += arr[i - j];

        if (i == 7) {
            /* Special case with the +1 constant. */
            arr[i] = (39 * sum) - (780 * arr[0]) + (seq_t)1;
        } else {
            /* Standard recurrence for i >= 8. */
            arr[i] = (39 * sum) - (780 * arr[i - 7]);
        }
    }
    last = n;
    return arr[n];
}
