/*
 * A060964  Table by antidiagonals where T(n,k) = n*T(n,k-1) - T(n,k-2) with
 *          T(n,0) = 2 and T(n,1) = n.
 * https://oeis.org/A060964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 39, i16 n <= 81, i32 n <= 194, i64 n <= 443,
 *            i128 n <= 1186, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Defines the OEIS sequence A060964.
   The underlying table T(n,k) satisfies:
        T(n,0) = 2,
        T(n,1) = n,
        T(n,k) = n·T(n,k‑1) – T(n,k‑2)   (k ≥ 2)
   The sequence a(n) lists the table entries by antidiagonals:
        For each s = 0,1,2,… the pairs (i , k) with i + k = s are
        output in the order i = 0 … s.  Thus
             a(s·(s+1)/2 + i) = T(i , s−i).
   This implementation maps an index n to its coordinates (row, col)
   and then computes T(row, col) by the recurrence in O(col) time.
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the antidiagonal s such that s*(s+1)/2 ≤ n <
       (s+1)*(s+2)/2.  Since s grows only like √(2n), a simple loop
       is sufficiently fast for n up to at least 10^5. */
    idx_t s = 0;
    while ( (s+1)*(s+2)/2 <= n )
        ++s;

    /* Offset of the first element of this antidiagonal. */
    idx_t offset = s*(s+1)/2;
    idx_t i = n - offset;        /* row index */
    idx_t k = s - i;             /* column index */

    /* Compute T(i,k) by the recurrence. */
    seq_t t0 = (seq_t)2;         /* T(i,0) */
    if (k == 0) return t0;

    seq_t t1 = (seq_t)i;         /* T(i,1) */
    if (k == 1) return t1;

    seq_t prev2 = t0, prev1 = t1, cur;
    for (idx_t j = 2; j <= k; ++j) {
        cur = ((seq_t)i) * prev1 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
