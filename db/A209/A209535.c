/*
 * A209535  Number of partitions of 0 of the form [x(1)+x(2)+...+x (j)] -
 *          [y(1)+y(2)+...+y(k)] where the x(i) are distinct positive integers
 *          <=n and the y(i) are distinct positive integers <= n.
 * https://oeis.org/A209535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{h=1}^n q(h)^2, where q(h) = number of partitions of h
 *            into distinct parts. Computed by dynamic programming
 *            (subset‑sum)
 * Range:     u8 n <= 9, u16 n <= 24, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)                    /* offset; not part of OEIS data */
        return 0;

    /* Allocate array for q[0..n] */
    seq_t *q = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!q) return 0;              /* allocation failure – unlikely */

    /* Initialization */
    for (idx_t i = 0; i <= n; ++i) q[i] = 0;
    q[0] = 1;                       /* empty partition */

    /* Subset‑sum DP: count distinct‑part partitions */
    for (idx_t k = 1; k <= n; ++k) {
        for (idx_t s = n; s >= k; --s) {
            q[s] += q[s - k];
            if (s == k) break;     /* avoid wrapping around when s is 0 */
        }
    }

    /* Accumulate sum of squares */
    seq_t result = 0;
    for (idx_t h = 1; h <= n; ++h) {
        seq_t t = q[h];
        result += t * t;
    }

    free(q);
    return result;
}
