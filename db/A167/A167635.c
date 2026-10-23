/*
 * A167635  Number of Dyck paths of semilength n, having no ascents and no
 *          descents of length 1, and having no peaks at odd level.
 * https://oeis.org/A167635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=0, a(2)=1, a(3)=0, a(4)=2, a(5)=0; for n≥6
 *            (n+1)*a(n) + a(n-1) + (-4n+5)*a(n-2) + (-2n+7)*a(n-3) + 3*a(n-4)
 *            + a(n-5) + (n-6)*a(n-6) = 0
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 31, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoisation array */
    static seq_t *arr = NULL;
    static idx_t  size = 0;

    if (size <= n) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(arr, new_size * sizeof(seq_t));
        if (!tmp) abort();
        arr = tmp;

        /* Initialise new entries */
        for (idx_t i = size; i < new_size; ++i) {
            if (i == 0)           arr[i] = 1;
            else if (i == 1)      arr[i] = 0;
            else if (i == 2)      arr[i] = 1;
            else if (i == 3)      arr[i] = 0;
            else if (i == 4)      arr[i] = 2;
            else if (i == 5)      arr[i] = 0;
            else { /* i >= 6 */
                seq_t a1 = arr[i - 1];
                seq_t a2 = arr[i - 2];
                seq_t a3 = arr[i - 3];
                seq_t a4 = arr[i - 4];
                seq_t a5 = arr[i - 5];
                seq_t a6 = arr[i - 6];

                /* Positive contributions (to sum) */
                seq_t pos = a1;                        /* a(n-1)            */
                pos += (seq_t)3 * a4;                  /* 3*a(n-4)          */
                pos += a5;                             /* a(n-5)            */
                pos += (seq_t)(i - 6) * a6;            /* (n-6)*a(n-6)      */

                /* Negative contributions (absolute value) */
                seq_t neg = (seq_t)(4 * i - 5) * a2;    /* (4n-5)*a(n-2)     */
                neg += (seq_t)(2 * i - 7) * a3;        /* (2n-7)*a(n-3)     */

                /* (n+1)*a(n) = neg - pos */
                /* We assume neg >= pos for n >= 6 */
                seq_t numer = neg - pos;
                seq_t denom = (seq_t)(i + 1);

                arr[i] = numer / denom;
            }
        }
        size = new_size;
    }
    return arr[n];
}
