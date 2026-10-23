/*
 * A171370  Sequence generated from Lim:_{n..inf.} M^n, M = an infinite lower
 *          triangular matrix with (1,3,3,3,...) in every column, shifted down
 *          twice.
 * https://oeis.org/A171370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 53, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence generated from Lim:_{n..inf.} M^n, with M an infinite lower triangular matrix
   whose columns are (1,3,3,3,...) shifted down twice.
   The nth term a(n) is computed by the recurrence

        a(0) = 1
        a(n) = Σ_{k=0}^{⌊n/2⌋} w(n,k) · a(k),  where
        w(n,k) = 1  if n = 2k,
                  3 otherwise.

   This recurrence exactly reproduces the first terms
   1,3,6,12,18,30,42,66,84,120,150,210,252,336,402,...
   and works for all n ≥ 0. */

#include <stdlib.h>

/* The build supplies seq_t and idx_t definitions. */
seq_t a(idx_t n)
{
    /* Special case: a(0) = 1 */
    if (n == 0) return (seq_t)1;

    /* Allocate temporary array to memoize a(k) up to n */
    seq_t *array = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!array) return (seq_t)0; /* Allocation failure falls back to 0 */

    array[0] = (seq_t)1;         /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t sum = 0;
        idx_t maxk = i >> 1;               /* floor(i/2) */
        for (idx_t k = 0; k <= maxk; ++k)
        {
            seq_t weight = (i == (k << 1)) ? (seq_t)1 : (seq_t)3;
            sum += weight * array[k];
        }
        array[i] = sum;
    }

    seq_t result = array[n];
    free(array);
    return result;
}
