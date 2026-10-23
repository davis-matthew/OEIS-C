/*
 * A022531  Nexus numbers (n+1)^15 - n^15.
 * https://oeis.org/A022531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^15 - n^15 (Nexus numbers)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 19, u128 n <= 465,
 *            big any n
 * Helpers:   power15
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t, if needed */
#include "../utils/power15.h"

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    seq_t pow_n   = power15(sn);       /* n^15   */
    seq_t pow_np1 = power15(sn + 1);    /* (n+1)^15 */
    return pow_np1 - pow_n;
}
