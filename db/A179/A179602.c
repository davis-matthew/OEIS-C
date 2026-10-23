/*
 * A179602  Eight white kings and one red king on a 3 X 3 chessboard. G.f.: (1
 *          + 2*x)/(1 - 3*x - 7*x^2).
 * https://oeis.org/A179602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) + 7*a(n-2), with a(0) = 1, a(1) = 5
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the n-th term of OEIS sequence A179602.
 * Uses the linear recurrence:
 *   a(0) = 1
 *   a(1) = 5
 *   a(n) = 3*a(n-1) + 7*a(n-2)  for n >= 2
 *
 * The algorithm is O(n) and uses only a few integer variables,
 * so it runs well below the time limit for n up to ~10 000 even
 * with arbitrary‑precision seq_t types.
 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    seq_t prev2 = (seq_t)1;  /* a(0)  */
    seq_t prev1 = (seq_t)5;  /* a(1)  */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = 3 * prev1 + 7 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
