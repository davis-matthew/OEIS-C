/*
 * A015465  q-Fibonacci numbers for q=8, scaling a(n-2).
 * https://oeis.org/A015465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 8^(n-2) * a(n-2), a(0)=0, a(1)=1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *seq = NULL;    /* cached sequence values */
static size_t seq_cap = 0;   /* number of values stored */
static seq_t pow8 = 0;       /* 8^(seq_cap-2), ready for next term */

seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (seq == NULL) {
        seq_cap = 2;            /* a(0) and a(1) */
        seq = seq_alloc(seq_cap);
        seq[0] = 0;
        seq[1] = 1;
        pow8  = 1;              /* 8^(2-2) = 1 */
    }

    /* extend cache if needed */
    size_t required = (size_t)n + 1;
    if (required > seq_cap) {
        size_t old_cap = seq_cap;
        seq_cap = required;
        seq = seq_realloc(seq, old_cap, seq_cap);

        for (idx_t i = old_cap; i <= n; ++i) {
            seq[(size_t)i] = seq[(size_t)i-1] + pow8 * seq[(size_t)i-2];
            pow8 *= 8;          /* update for next index */
        }
    }

    return seq[(size_t)n];
}
