/*
 * A037092  Number of triples {i,j,k}, i>1, j>1, k>1, such that i*j*k < n^3.
 * https://oeis.org/A037092
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=2..n-1} Sum_{j=i..⌊√((n³-1)/i)⌋}
 *            (⌊(n³-1)/(i·j)⌋-j+1)
 * Range:     u8 n <= 5, u16 n <= 23, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* n ≥ 2. For n < 3 the summation is empty, so return 0. */
    if (n < 3)
        return (seq_t)0;

    seq_t total = (seq_t)0;
    seq_t N = (seq_t)n;
    seq_t limit = N * N * N - (seq_t)1;   /* n³ – 1 */

    for (idx_t i = 2; i < n; ++i) {
        seq_t i_seq = (seq_t)i;
        seq_t den = limit / i_seq;                 /* (n³‑1)/i      */
        seq_t sqrt_den = floor_sqrt(den);           /* ⌊√((n³‑1)/i)  */
        idx_t j_max = (idx_t)sqrt_den;              /* upper bound for j */

        for (idx_t j = i; j <= j_max; ++j) {
            seq_t j_seq = (seq_t)j;
            seq_t kmax = den / j_seq;               /* ⌊(n³‑1)/(i·j)⌋  */
            seq_t add = kmax - j_seq + (seq_t)1;
            total += add;
        }
    }

    return total;
}
