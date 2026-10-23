/*
 * A180312  Number of solutions to n = x + 4*y + 4*z in triangular numbers.
 * https://oeis.org/A180312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of triples (a,b,c) such that * n = T_a + 4*T_b +
 *            4*T_c, * where T_k = k*(k+1)/2 (triangular numbers)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_tri_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_tri_value.h"

seq_t a(idx_t n)
{
    /* Determine how many triangular numbers are <= n */
    idx_t tcnt = 0;
    for (idx_t k = 0;; ++k) {
        idx_t T = (k * (k + 1)) / 2;
        if (T > n)
            break;
        ++tcnt;
    }

    /* Allocate array of triangular numbers */
    idx_t *tri = (idx_t *)malloc(sizeof(idx_t) * tcnt);
    if (!tri)
        return (seq_t)0;  /* allocation failure – return 0 */

    for (idx_t i = 0; i < tcnt; ++i) {
        idx_t k = i;            /* k starts at 0 */
        tri[i] = (k * (k + 1)) / 2;
    }

    idx_t count = 0;

    /* Enumerate all possible T_a */
    for (idx_t ai = 0; ai < tcnt; ++ai) {
        idx_t Ti = tri[ai];
        idx_t rem1 = n - Ti;
        if ((rem1 & 3) != 0)          /* n - T_a must be divisible by 4 */
            continue;
        idx_t t = rem1 >> 2;          /* (n - T_a) / 4 */

        /* For each T_b such that T_b <= t, check if t - T_b is triangular */
        for (idx_t bj = 0; bj < tcnt; ++bj) {
            idx_t Tbj = tri[bj];
            if (Tbj > t)
                break;
            idx_t rem2 = t - Tbj;
            if (is_tri_value(rem2, tri, tcnt))
                ++count;              /* ordered pair (b,c) counted */
        }
    }

    free(tri);
    return (seq_t)count;
}
