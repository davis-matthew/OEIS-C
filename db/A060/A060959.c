/*
 * A060959  Table by antidiagonals of generalized Fibonacci numbers: T(n,k) =
 *          T(n,k-1) + n*T(n,k-2) with T(n,0)=0 and T(n,1)=1.
 * https://oeis.org/A060959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where T(i,0)=0, T(i,1)=1,
 *            T(i,k)=T(i,k-1)+i*T(i,k-2), and (i,j) is obtained by mapping the
 *            index n to an antidiagonal of the two‑dimensional table T
 * Range:     u8 n <= 79, u16 n <= 173, u32 n <= 411, u64 n <= 1088,
 *            u128 n <= 3015, big any n
 * Helpers:   generalized_fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* required for idx_t and seq_t types */
#include <stdint.h>
#include "../utils/generalized_fib.h"

/* ----- main sequence function ----- */
seq_t a(idx_t n)
{
    /* find s such that s*(s+1)/2 <= n < (s+1)*(s+2)/2   */
    idx_t s = 0;
    while ((s + 1) * (s + 2) / 2 <= n) ++s;

    /* preceding count = s*(s+1)/2 */
    idx_t prev = s * (s + 1) / 2;
    idx_t offset = n - prev;          /* offset in current antidiagonal, 0 <= offset <= s */
    idx_t i = offset;                 /* row index */
    idx_t j = s - offset;             /* column index */

    return generalized_fib(i, j);
}
