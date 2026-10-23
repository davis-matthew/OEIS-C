/*
 * A035015  Period of continued fraction for square root of n-th squarefree
 *          integer.
 * https://oeis.org/A035015
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = A003285(A005117(n))
 * Range:     u8 n <= 39, u16 n <= 8711, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the period of the simple continued fraction of the square root
 * of the n-th square‑free integer.  It uses only integer arithmetic
 * and the utilities provided by the framework. */

#include <stdbool.h>
#include "../utils/is_squarefree.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* offset starts at 2 */

    /* Find the n-th square‑free integer. */
    seq_t d = 0;
    idx_t cnt = 0;
    for (idx_t k = 1;; ++k) {
        seq_t x = (seq_t)k;                /* k fits in idx_t → seq_t */
        if (is_squarefree(x)) {
            ++cnt;
            if (cnt == n) {
                d = x;
                break;
            }
        }
    }

    /* If d is a perfect square (should not happen for square‑free d>1),
     * the continued fraction is finite with period 0. */
    seq_t a0 = floor_sqrt(d);
    if (a0 * a0 == d) return 0;

    /* Standard algorithm to determine the period length of √d. */
    seq_t m   = 0;
    seq_t d1  = 1;
    seq_t a   = a0;
    seq_t period = 0;
    seq_t double_a0 = a0 + a0;      /* 2*a0 */

    while (1) {
        m   = d1 * a - m;
        d1  = (d - m * m) / d1;
        a   = (a0 + m) / d1;
        ++period;
        if (a == double_a0) break;
    }

    return period;
}
