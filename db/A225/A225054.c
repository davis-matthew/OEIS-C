/*
 * A225054  Triangle read by rows: Eulerian numbers T(n, k) = A173018(n + 1,
 *          k) reduced mod n + 1.
 * https://oeis.org/A225054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Eulerian(r+1, k) mod (r+1) where r(r+1)/2 <= n <
 *            (r+1)(r+2)/2 and k = n - r(r+1)/2
 * Range:     u8 n <= 16, u16 n <= 38, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* Find the row r that contains index n. */
    idx_t r = (idx_t)((sqrt((long double)(8.0L * (long double)n + 1.0L)) - 1.0L) / 2.0L);
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;

    idx_t k = n - r * (r + 1) / 2;   /* column inside the row */
    idx_t M = r + 1;                 /* row length = modulus */

    /* Special case: first row has a single entry 0. */
    if (M == 1)
        return (seq_t)0;

    /* Build Eulerian numbers row by row, exactly, using arbitrary‑precision arithmetic. */
    seq_t *prev = (seq_t*)calloc(1, sizeof(seq_t));  /* row 1: [1] */
    prev[0] = (seq_t)1;

    for (idx_t m = 2; m <= M; ++m) {
        seq_t *curr = (seq_t*)calloc((size_t)m, sizeof(seq_t));
        curr[0] = (seq_t)1;
        curr[m - 1] = (seq_t)1;

        for (idx_t j = 1; j < m - 1; ++j) {
            seq_t part1 = ((seq_t)(m - j)) * prev[j - 1];
            seq_t part2 = ((seq_t)(j + 1)) * prev[j];
            curr[j] = part1 + part2;  /* exact value, no modulus yet */
        }

        free(prev);
        prev = curr;                 /* prepare for next iteration */
    }

    /* prev now holds the exact Eulerian numbers of row M.
       Reduce the required entry modulo M and return. */
    seq_t result = prev[k] % (seq_t)M;
    free(prev);
    return result;
}
