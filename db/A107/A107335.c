/*
 * A107335  1-limiting word of the morphism 1->113231, 2->3, 3->1.
 * https://oeis.org/A107335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   symbol_at
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: 1‑limiting word of the morphism 1→113231, 2→3, 3→1
   (recursively descend the morphism’s tree). */

#include <stddef.h>  /* size_t for static array dimension */
#include "../utils/symbol_at.h"

seq_t a(idx_t n)
{
    /* Convert the 64‑bit index to a seq_t once, so all length arithmetic
       is performed with the same type. */
    seq_t n_seq = (seq_t)n;

    /* Length table: len[d][c] is the number of symbols obtained from
       symbol c after d applications of the morphism.  depth 0 → no
       substitution; each entry is always a positive integer. */
    static seq_t len[128][4];            /* indices 1‑3 used; depth up to 127 */
    static int initialized = 0;
    if (!initialized) {
        len[0][1] = len[0][2] = len[0][3] = (seq_t)1;
        initialized = 1;
    }

    /* Find a depth that guarantees the root string (starting from 1)
       is long enough to cover position n. */
    int depth = 0;
    while (len[depth][1] <= n_seq) {
        int d = ++depth;
        /* Ensure we do not exceed the preallocated table size. */
        if (d >= 128) {
            /* For the given constraints the sequence grows fast; this
               situation only occurs for astronomically large n.  We
               guard against it by treating the length as infinite. */
            break;
        }
        seq_t l1 = 3 * len[d - 1][1] + 2 * len[d - 1][3] + len[d - 1][2];
        seq_t l2 = len[d - 1][3];
        seq_t l3 = len[d - 1][1];
        len[d][1] = l1;
        len[d][2] = l2;
        len[d][3] = l3;
    }

    /* Descend the morphism tree to retrieve the symbol at position n. */
    return symbol_at(depth, 1, n_seq, len);
}
