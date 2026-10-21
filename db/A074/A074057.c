/*
 * A074057  a(n) = 2*phi(n-2)-(n-1).
 * https://oeis.org/A074057
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 2*phi(n-2) - (n-1)
 * Range:     i8 n <= 85, i16 n <= 85, i32 n <= 85, i64 n <= 85, i128 n <= 85,
 *            big any n
 * Helpers:   phi_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/phi_u64.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 3.  Behaviour for smaller n
       is irrelevant; return 0 to avoid unsigned underflow. */
    if (n < 3) return (seq_t)0;

    /* Compute φ(n‑2).  phi_u64 returns a seq_t value. */
    seq_t phi_val = phi_u64(n - 2);

    /* a(n) = 2*φ(n‑2) – (n‑1) */
    seq_t res = (seq_t)2 * phi_val - ((seq_t)n - 1);
    return res;
}
