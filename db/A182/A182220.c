/*
 * A182220  Largest number k such that there exists an extensional acyclic
 *          digraph on n labeled nodes with k sources.
 * https://oeis.org/A182220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 (n = 1); for n ≥ 2: a(n) = (n-1) - ⌊log₂(n-1)⌋
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The utility floor_log2_uint64 is provided in the build environment. */
#include <stdint.h>
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)1)      /* a(1) = 1 */
        return (seq_t)1;

    idx_t m  = n - (idx_t)1;          /* m = n-1  (fitting in idx_t) */
    idx_t lg = floor_log2_uint64(m);  /* ⌊log₂(m)⌋  */
    return (seq_t)(m - lg);           /* a(n) = m - ⌊log₂(m)⌋ */
}
