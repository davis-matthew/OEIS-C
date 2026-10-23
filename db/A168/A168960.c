/*
 * A168960  Number of reduced words of length n in Coxeter group on 43
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^22 = I.
 * https://oeis.org/A168960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A168960:
   Number of reduced words of length n in the Coxeter group on 43 generators
   with relations (S_i)^2 = (S_i S_j)^22 = I.
   The ordinary generating function is
          (t^22 + 2*t^21 + … + 2*t + 1) /
          (861*t^22 - 41*t^21 - … - 41*t + 1).
   From this we obtain the linear recurrence:
      a(0) = 1
      a(n) - 41*∑_{k=1}^{min(21,n)} a(n-k) = 2            for 1 ≤ n ≤ 21
      a(22) - 41*∑_{k=1}^{21} a(22-k) + 861*a(0) = 1
      a(n) - 41*∑_{k=1}^{21} a(n-k) + 861*a(n-22) = 0    for n ≥ 23
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Allocate array for a[0..n] */
    seq_t *arr = (seq_t*)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!arr) return (seq_t)0;          /* allocation failure – return 0 */

    arr[0] = (seq_t)1;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        idx_t limit = (i >= 21) ? 21 : i;          /* number of terms to sum */
        for (idx_t k = 1; k <= limit; ++k)
            sum += arr[i - k];

        if (i <= 21) {
            /* a_i = 2 + 41*sum  */
            arr[i] = (seq_t)2 + (seq_t)41 * sum;
        } else if (i == 22) {
            /* a_22 = 41*sum - 860  (because 861*a_0 = 861) */
            arr[i] = (seq_t)41 * sum - (seq_t)860;
        } else {
            /* a_i = 41*sum - 861*a_{i-22} */
            arr[i] = (seq_t)41 * sum - (seq_t)861 * arr[i - 22];
        }
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
