/*
 * A117411  Skew triangle associated to the Euler numbers.
 * https://oeis.org/A117411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 24, i16 n <= 72, i32 n <= 266, i64 n <= 929,
 *            i128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n) is defined as the flattening of the skew triangle T(n,k)
   where
        T(0,0) = 1,
        T(1,0) = 0,  T(1,1) = 1,
     T(n,k) = 2*T(n-1,k-1) – 4*T(n-2,k-1) – T(n-2,k-2)  for 2 ≤ n and 1 ≤ k < n,
     and T(n,n) = 1.
   a(n) is obtained by writing the rows of T in order, starting with n = 0.  */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while (((__uint128_t)r * (r + 1) / 2) <= n) ++r;
    --r;
    idx_t c = n - ((idx_t)((__uint128_t)r * (r + 1) / 2));

    /* Trivial case */
    if (r == 0) return (seq_t)1;

    /* Allocate and compute rows iteratively */
    seq_t *prev2 = NULL;   /* row r-2 */
    seq_t *prev1 = NULL;   /* row r-1 */
    seq_t *curr  = NULL;   /* current row */

    for (idx_t i = 0; i <= r; ++i) {
        curr = (seq_t *)calloc((size_t)(i + 1), sizeof(seq_t));
        if (curr == NULL) return (seq_t)0;  /* allocation failure */

        if (i == 0) {
            /* T(0,0) = 1 */
            curr[0] = (seq_t)1;
        } else if (i == 1) {
            /* T(1,0) = 0, T(1,1) = 1 */
            curr[0] = (seq_t)0;
            curr[1] = (seq_t)1;
        } else {
            /* General row */
            curr[0] = (seq_t)0;
            curr[i] = (seq_t)1;   /* T(i,i) = 1 */
            for (idx_t k = 1; k < i; ++k) {
                seq_t t1 = prev1 ? prev1[k - 1] : (seq_t)0;
                seq_t t2 = (prev2 ? prev2[k - 1] : (seq_t)0);
                seq_t t3 = (k >= 2 ? (prev2 ? prev2[k - 2] : (seq_t)0) : (seq_t)0);
                curr[k] = (seq_t)2 * t1 - (seq_t)4 * t2 - t3;
            }
        }

        /* Advance pointers: prev2 <- prev1, prev1 <- curr */
        free(prev2);
        prev2 = prev1;
        prev1 = curr;
    }

    seq_t result = prev1[c];
    free(prev2);
    free(prev1);
    return result;
}
