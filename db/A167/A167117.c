/*
 * A167117  Number of reduced words of length n in Coxeter group on 16
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^14 = I.
 * https://oeis.org/A167117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 14*(a(n-1)+...+a(n-13)) – 105*a(n-14) + c(n)
   where c(0)=1, c(1..13)=2, c(14)=1, c(n)=0 for n>14. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* allocate array of size n+1 */
    idx_t size = n + 1;
    seq_t *arr = (seq_t *)malloc(size * sizeof(seq_t));
    if (!arr) return 0;   /* allocation failed */

    for (idx_t i = 0; i < size; ++i)
    {
        /* coefficient c(i) from the numerator of the generating function */
        seq_t c = (i == 0 || i == 14) ? 1 : ((i <= 13) ? 2 : 0);

        seq_t val = c;

        /* add 14 * a[i-1] .. 14 * a[i-13]   (only for existing indices) */
        idx_t maxj = 13;
        if (maxj > i) maxj = i;
        for (idx_t j = 1; j <= maxj; ++j)
        {
            val += (seq_t)14 * arr[i - j];
        }

        /* subtract 105 * a[i-14] when i >= 14 */
        if (i >= 14)
        {
            val -= (seq_t)105 * arr[i - 14];
        }

        arr[i] = val;
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
