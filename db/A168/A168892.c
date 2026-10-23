/*
 * A168892  Number of reduced words of length n in Coxeter group on 23
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^21 = I.
 * https://oeis.org/A168892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A168892: number of reduced words of length n in the Coxeter group
   with 23 generators S_i, relations S_i^2 = (S_i S_j)^21 = I.
   The generating function is
        G(t) = (t^21 + 2t^20 + ... + 2t + 1) /
               (231t^21 - 21t^20 - 21t^19 - ... - 21t + 1).
   From G(t) we obtain the linear recurrence (valid for n ≥ 21)

        a(n) = 21*a(n‑1) + 21*a(n‑2) + … + 21*a(n‑20) - 231*a(n‑21).

   Initial values for 0 ≤ n ≤ 20 are given by
        a(0) = 1,
        a(n) = 2 + 21*(a(0)+…+a(n‑1))        (1 ≤ n ≤ 20).
   For n = 21 the formula is

        a(21) = 1 + 21*(a(1)+…+a(20)) – 231*a(0).

   The function below implements this recurrence without using any
   external helper functions.  It works for all builds, whether
   seq_t is a builtin unsigned integer type or an arbitrary‑precision
   class.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* a(0) is trivial */
    if (n == 0)
        return 1;

    /* Allocate space for a[0..n] */
    seq_t *a = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!a)
        return 0;          /* Memory failure – return 0 as a safe sentinel */

    a[0] = 1;              /* a(0) */
    seq_t sum_pref = a[0]; /* running sum of a[0..i-1] for i≤20 */

    /* Compute a(1)..a(20) using the simplified formula */
    idx_t i;
    for (i = 1; i <= 20 && i <= n; ++i) {
        a[i] = (seq_t)2 + (seq_t)21 * sum_pref;
        sum_pref += a[i];
    }
    if (i > n) {          /* we have reached the requested n */
        seq_t val = a[n];
        free(a);
        return val;
    }

    /* Compute a(21) using the full recurrence relation */
    seq_t sum20 = 0;
    for (i = 1; i <= 20; ++i)
        sum20 += a[i];
    a[21] = (seq_t)1 + (seq_t)21 * sum20 - (seq_t)231 * a[0];
    if (n == 21) {
        seq_t val = a[21];
        free(a);
        return val;
    }

    /* Compute a(22)..a(n) using the 21‑term linear recurrence */
    for (i = 22; i <= n; ++i) {
        seq_t sum20_cur = 0;
        idx_t j;
        for (j = 1; j <= 20; ++j)
            sum20_cur += a[i - j];
        a[i] = (seq_t)21 * sum20_cur - (seq_t)231 * a[i - 21];
    }

    seq_t result = a[n];
    free(a);
    return result;
}
