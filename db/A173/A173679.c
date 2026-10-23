/*
 * A173679  Number of ways of writing n as a sum of 5 nonnegative cubes.
 * https://oeis.org/A173679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 755, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A173679:
   a(n) = coefficient of q^n in (Σ_{m≥0} q^{m^3})^5
   The routine performs 5 successive convolutions over the list of
   non‑negative cubes not exceeding n, using only integer arithmetic
   on type seq_t.  All intermediate arrays are allocated with
   calloc/malloc and cast to (seq_t *) so that the code compiles
   in both C11 and C++17.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Generate all cubes ≤ n */
    idx_t *cubes = NULL;
    size_t cube_cnt = 0;
    for (idx_t m = 0; ; ++m) {
        idx_t cube = m * m * m;
        if (cube > n) break;
        idx_t *tmp = (idx_t *)realloc(cubes, (cube_cnt + 1) * sizeof(idx_t));
        if (!tmp) { free(cubes); return 0; }
        cubes = tmp;
        cubes[cube_cnt++] = cube;
    }

    /* Allocate convolution buffers */
    seq_t *prev = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!prev) { free(cubes); return 0; }
    prev[0] = 1;                      /* (∑ q^{m^3})^0 has coefficient 1 at 0 */

    seq_t *cur = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!cur) { free(prev); free(cubes); return 0; }

    /* Perform 5 successive convolutions */
    for (idx_t rep = 0; rep < 5; ++rep) {
        for (idx_t i = 0; i <= n; ++i) {
            seq_t sum = 0;
            for (size_t ci = 0; ci < cube_cnt && cubes[ci] <= i; ++ci) {
                sum += prev[i - cubes[ci]];
            }
            cur[i] = sum;
        }
        /* Prepare for next iteration */
        seq_t *tmp = prev;
        prev = cur;
        cur = tmp;
    }

    seq_t result = prev[n];
    free(cur);
    free(prev);
    free(cubes);
    return result;
}
