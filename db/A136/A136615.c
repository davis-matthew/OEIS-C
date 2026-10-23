/*
 * A136615  Fold-switch-fold sequence defined by McFarlane and Withers for
 *          m=3: Let A(n) = If[Mod[A(n - 1), 2] == 0, A(n - 1)/2, (m - A(n -
 *          1))2]; a(n)= If[ Mod[A(n - 1), 2] == 0, a(n - 1)/2, (Pi - a(n -
 *          1))/2].
 * https://oeis.org/A136615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 2, a(n) = 0 for 3 ≤ n ≤ 5. For n ≥ 6, write n =
 *            3k + 3 with k ≥ 1. Let N₁ = 1 and N_{i+1} = 8·N_i + (−1)^i for i
 *            ≥ 1. Then a(n) = 16 · N_k
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 1) {
        return (seq_t)1;
    }
    if (n == 2) {
        return (seq_t)2;
    }
    if (n >= 3 && n <= 5) {
        return (seq_t)0;
    }
    /* n >= 6: compute k = (n-3)/3, integer division yields 1 for 6..8, 2 for 9..11, etc. */
    idx_t k = (n - 3) / 3;        /* k >= 1 */
    seq_t N = (seq_t)1;           /* N₁ */
    for (idx_t i = 1; i < k; ++i) {
        N *= (seq_t)8;
        if (i & 1) {
            N -= (seq_t)1;        /* subtract 1 when i is odd */
        } else {
            N += (seq_t)1;        /* add 1 when i is even */
        }
    }
    return N * (seq_t)16;
}
