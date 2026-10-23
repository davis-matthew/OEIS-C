/*
 * A036036  Triangle read by rows in which row n lists all the parts of all
 *          reversed partitions of n, sorted first by length and then
 *          lexicographically.
 * https://oeis.org/A036036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, tabf, look
 * Formula:   not stated
 * Range:     u8 n <= 8600, u16 n <= 8600, u32 n <= 8600, u64 n <= 8600,
 *            u128 n <= 8600, big any n
 * Helpers:   gen_k_parts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A036036 – Flattened parts of all reversed partitions.
    The sequence is produced by iterating m = 1, 2, …,
    generating all partitions of m in ascending order of parts
    (the reversed partitions), ordering partitions by increasing
    length and then lexicographically.
    The parts of each partition are output in their natural order.
    a(n) is the n‑th part in this flattened list (1‑based). */

#include <stddef.h>  /* for size_t */
#include "../utils/gen_k_parts.h"

seq_t a(idx_t target)
{
    /* large enough to hold partitions up to a few hundred parts */
    static idx_t parts[512];
    idx_t  curr_index = 0;
    seq_t answer = 0;

    for (idx_t m = 1; ; ++m) {                 /* iterate over rows m = 1,2,… */
        for (idx_t k = 1; k <= m; ++k) {       /* length of the partition */
            gen_k_parts(m, k, 1, 0, parts, (size_t)k,
                        &curr_index, target, &answer);
            if (curr_index >= target)          /* found the term */
                return answer;
        }
    }
    /* unreachable, but return 0 to silence compiler warnings */
    return answer;
}
