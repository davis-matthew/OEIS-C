/*
 * A168753  Number of reduced words of length n in Coxeter group on 28
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^18 = I.
 * https://oeis.org/A168753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS sequence A168753: Number of reduced words of length n in a Coxeter
   group on 28 generators with relations (S_i)^2 = (S_i S_j)^18 = I.
   
   It satisfies the rational generating function
   G(t) = (t^18 + 2*t^17 + … + 2*t + 1) /
          (351*t^18 - 26*t^17 - … - 26*t + 1).
   
   From this the sequence obeys the linear recurrence
      a(0) = 1,
      for 1 ≤ n ≤ 17:   a(n) = 26 * Σ_{k=0}^{n-1} a(k) + 2
      a(18)            = 26 * Σ_{k=1}^{17} a(k) - 351*a(0) + 1
      for n ≥ 19:       a(n) = 26 * Σ_{k=n-17}^{n-1} a(k) - 351*a(n-18)
   
   The following implementation computes a(n) directly using this recurrence
   and maintains a sliding window of the last 17 terms for the
   n ≥ 19 case.  All arithmetic is performed with the generic type
   seq_t, so it works for 8‑ to 128‑bit integers as well as arbitrary
   precision big integers.
*/

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Allocate an array for terms 0..n.  Casting the malloc result is
       required for builds where seq_t is a C++ class with a non-trivial
       constructor.  We will assign to every element before use. */
    size_t len = (size_t)n + 1;
    seq_t *arr = (seq_t *)malloc(len * sizeof(seq_t));
    if (!arr) return (seq_t)0;          /* allocation failure */

    /* Term a(0) */
    arr[0] = (seq_t)1;
    seq_t cum = arr[0];                 /* cumulative sum Σ_{k=0}^{i} a(k) */

    idx_t i;

    /* 1 ≤ n ≤ 17 */
    for (i = 1; i <= 17 && i <= n; ++i) {
        seq_t val = ((seq_t)26) * cum + (seq_t)2;
        arr[i] = val;
        cum += val;
    }

    if (n >= 18) {
        /* Prepare window sum Σ_{k=1}^{17} a(k) for the n = 18 computation */
        seq_t window_sum = cum - arr[0];          /* a[1] .. a[17] */

        /* a(18) */
        seq_t val18 = ((seq_t)26) * window_sum
                     - ((seq_t)351) * arr[0] + (seq_t)1;
        arr[18] = val18;
        cum += val18;

        /* Update window sum to be ready for the next term (n = 19) */
        window_sum += arr[18] - arr[1];

        /* n ≥ 19 */
        for (i = 19; i <= n; ++i) {
            seq_t val = ((seq_t)26) * window_sum
                        - ((seq_t)351) * arr[i - 18];
            arr[i] = val;
            cum += val;
            /* Slide the window: remove a[i-17], add a[i] */
            window_sum += arr[i] - arr[i - 17];
        }
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
