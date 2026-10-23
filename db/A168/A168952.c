/*
 * A168952  Number of reduced words of length n in Coxeter group on 35
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^22 = I.
 * https://oeis.org/A168952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A168952
   Figure: Number of reduced words of length n in the Coxeter group on 35 generators
   with relations (S_i)^2 = (S_i S_j)^2 = I.

   Recurrence (order 22):
   a(n) = 33 * Σ_{k=1}^{min(21,n)} a(n−k)   +   561 * a(n−22)   +   N_n
   where N_n = 1  for n = 0 or 22,
            N_n = 2  for 1 ≤ n ≤ 21,
            N_n = 0  otherwise.

   Initial values: a(0)=1, a(1)=35, a(2)=1190, a(3)=40460, …   (see OEIS entry)
*/

#include <stdlib.h>
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Allocate temporary storage for the first n+1 sequence values */
    seq_t *arr = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!arr)
        return (seq_t)0;          /* allocation failed – unlikely for n≤10000 */

    arr[0] = (seq_t)1;

    for (idx_t i = 1; i <= n; ++i)
    {
        /* Sum of the up to 21 previous terms.  (arr[k] is 0 for k < 0.) */
        seq_t sum = 0;
        idx_t kmax = (i < 21 ? i : 21);
        for (idx_t k = 1; k <= kmax; ++k)
            sum = sum + arr[i - k];

        /* Start with 33 * sum */
        seq_t val = seq_mul((seq_t)33, sum);

        /* Add the N_n contribution */
        if (i == 22)
            val = val + 1;
        else if (i <= 21)
            val = val + 2;

        /* Add the 561 * a(n−22) term when available */
        if (i >= 22)
            val = val + seq_mul((seq_t)561, arr[i - 22]);

        arr[i] = val;
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
