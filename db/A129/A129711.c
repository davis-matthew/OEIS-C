/*
 * A129711  Triangle read by rows: T(n,k) is the number of Fibonacci binary
 *          words of length n and starting with exactly k 01's
 *          (0<=k<=floor(n/2)). A Fibonacci binary word is a binary word
 *          having no 00 subword.
 * https://oeis.org/A129711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   The flattened sequence a(n) corresponds to the triangle T(n,k)
 *            read by rows, where 0 ≤ k ≤ ⌊n/2⌋. T(n,k) = 2 if 2*k+1 == n, =
 *            F(n-2*k+1) otherwise, and F(j) denotes Fibonacci numbers with
 *            F(0)=0, F(1)=1. This implements the OEIS sequence A129711
 * Range:     u8 n <= 48, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Determine the row r and column k corresponding to the
       flattened index n.  Row lengths are 1+⌊r/2⌋. */
    idx_t r = 0;
    idx_t cum = 0;              /* total entries before row r */
    while (cum + 1 + r / 2 <= n) {
        cum += 1 + r / 2;
        r++;
    }
    idx_t k = n - cum;

    /* Compute the triangle entry. */
    if (2 * k + 1 == r) {
        return (seq_t)2;
    } else {
        idx_t fib_idx = r - 2 * k + 1;
        return fib(fib_idx);
    }
}
