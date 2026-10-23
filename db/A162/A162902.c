/*
 * A162902  An increasing sequence of alternatingly squarefree and
 *          nonsquarefree numbers.
 * https://oeis.org/A162902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 143, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A162902: increasing sequence of alternately squarefree and nonsquarefree numbers. */

#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    /* The sequence starts with a(1) = 1 (squarefree), then alternates.
       We compute the first n terms each time this function is called. */
    seq_t cur = 0;                 /* value before the first term */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t cand = cur + 1;      /* start checking from the next integer */
        if ((i & 1) == 1) {        /* odd index → squarefree */
            while (!is_squarefree(cand))
                ++cand;
        } else {                   /* even index → non‑squarefree */
            while (is_squarefree(cand))
                ++cand;
        }
        cur = cand;                /* update current value */
    }
    return cur;
}
