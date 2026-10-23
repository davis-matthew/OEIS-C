/*
 * A123490  Triangle whose k-th column satisfies a(n) =
 *          (k+3)*a(n-1)-(k+2)*a(n-2).
 * https://oeis.org/A123490
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is the linear‐indexed triangular array T(n,k) with T(r,k) =
 *            ((k+2)^(r-k) + k)/(k+1) (0 ≤ k ≤ r), where row r starts at index
 *            r*(r+1)/2
 * Range:     u8 n <= 28, u16 n <= 67, u32 n <= 173, u64 n <= 439,
 *            u128 n <= 1136, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n‑th term of OEIS A123490. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* locate the row r and column k for index n */
    idx_t r = 0;                     /* row index (0‑based)   */
    /* triangular number of terms up to row r (inclusive) */
    while (((seq_t)(r + 1) * (r + 2)) / 2 <= n)
        ++r;

    idx_t start = r * (r + 1) / 2;   /* first index of row r     */
    idx_t k = n - start;             /* column within the row     */
    idx_t m = r - k;                 /* exponent in the definition */

    /* Special case m = 0 → T(r,r) = 1. */
    if (m == 0)
        return (seq_t)1;

    /* Compute T(r,k) = 1 + sum_{i=0}^{m-1} (k+2)^i   */
    seq_t base = (seq_t)(k + 2);
    seq_t pow  = (seq_t)1;   /* current power of base   */
    seq_t sum  = (seq_t)1;   /* start with base^0 = 1   */

    /* Add powers base^1 through base^(m-1). */
    for (idx_t i = 1; i < m; ++i) {
        pow *= base;   /* pow becomes base^i   */
        sum += pow;
    }

    /* Final value: 1 + sum */
    return sum + (seq_t)1;
}
