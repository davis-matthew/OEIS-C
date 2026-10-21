/*
 * A236307  Discriminants d such that the ring of algebraic integers of
 *          Q(sqrt(-d)) is not a unique factorization domain.
 * https://oeis.org/A236307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth squarefree integer that is not a Heegner number (the
 *            Heegner numbers are 1,2,3,7,11,19,43,67,163)
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    /* n >= 1; handle zero or negative gracefully (undefined behaviour otherwise) */
    if (n == 0)
        return 0;

    /* Heegner numbers list */
    static const seq_t heegner[] = {
        (seq_t)1, (seq_t)2, (seq_t)3, (seq_t)7,
        (seq_t)11, (seq_t)19, (seq_t)43,
        (seq_t)67, (seq_t)163
    };
    const size_t heegner_cnt = sizeof(heegner) / sizeof(heegner[0]);

    idx_t    count = 0;           /* how many terms we have found */
    seq_t    val   = (seq_t)1;    /* start from 1, the smallest positive integer */

    for (;; ++val) {
        /* Skip non‑squarefree numbers */
        if (!is_squarefree(val))
            continue;

        /* Skip Heegner numbers */
        bool skip = false;
        for (size_t i = 0; i < heegner_cnt; ++i) {
            if (val == heegner[i]) {
                skip = true;
                break;
            }
        }
        if (skip)
            continue;

        /* Valid term found */
        ++count;
        if (count == n)
            return val;
    }
}
