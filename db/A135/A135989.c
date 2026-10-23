/*
 * A135989  a(n) = 6*n + 3 + 90*floor((6*n+3)/10).
 * https://oeis.org/A135989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*n + 3 + 90*floor((6*n+3)/10)
 * Range:     u8 n <= 4, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute in 64‑bit arithmetic to avoid overflow for large n */
    idx_t base   = n * (idx_t)6 + (idx_t)3;   /* 6*n + 3            */
    idx_t result = base + (idx_t)90 * (base / (idx_t)10);
    return (seq_t)result;
}
