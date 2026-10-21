/*
 * A206571  Number of nX2 0..3 arrays avoiding the pattern z-1 z-1 z in any
 *          row, column or nw-to-se diagonal.
 * https://oeis.org/A206571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (f(n))², where f(n) = number of n‑length sequences over
 *            {0,1,2,3} that avoid the forbidden patterns 0 0 1, 1 1 2, 2 2 3.
 *            f(1) = 4 → a(1) = 16 f(2) = 16 → a(2) = 256 For n ≥ 3 compute
 *            f(n) by a DFA on the last two symbols
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   compute_f_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t (not really used here but harmless) */
#include "../utils/compute_f_n.h"

seq_t
a(idx_t n)
{
    seq_t fn = compute_f_n(n);
    return fn * fn;
}
