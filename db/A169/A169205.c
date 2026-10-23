/*
 * A169205  Number of reduced words of length n in Coxeter group on 48
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^27 = I.
 * https://oeis.org/A169205
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

/* Sequence A169205: number of reduced words of length n in the Coxeter
   group on 48 generators with relations (S_i)^2 = (S_i S_j)^27 = I.
   Recurrence:
        a(0) = 1
        a(n) = 2 for 1 ≤ n ≤ 26
        a(27) = 1
        For all n:
            a(n) = rhs(n) – 1081·a(n–27) + 46·∑_{i=1}^{26} a(n–i)
   where rhs(n) = 1 if n = 0 or n = 27,
                 2 if 1 ≤ n ≤ 26,
                 0 otherwise.  For n < 0 the terms a(k) are taken as 0.  */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Allocate array [0..n] */
    size_t len = (size_t)n + 1;
    seq_t *arr = (seq_t*)malloc(len * sizeof(seq_t));
    if (!arr) { return 0; } /* allocation failure – unlikely for n ≤ 10000 */

    for (idx_t i = 0; i <= n; ++i) {
        /* rhs(n) */
        seq_t rhs;
        if (i == 0)                 rhs = 1;
        else if (i <= 26)           rhs = 2;
        else if (i == 27)           rhs = 1;
        else                        rhs = 0;

        /* sum of previous 26 terms */
        seq_t sum = 0;
        if (i >= 1) {
            idx_t m = (i < 26) ? i : 26;   /* number of available terms to add */
            for (idx_t j = 1; j <= m; ++j) {
                sum += arr[i - j];
            }
        }

        /* term a(n-27) */
        seq_t term = 0;
        if (i >= 27) {
            term = 1081 * arr[i - 27];
        }

        /* compute a(n) */
        arr[i] = rhs + 46 * sum - term;
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
