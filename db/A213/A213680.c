/*
 * A213680  a(n) = 2*a(n-1)^2/3-3 with a(0)=6.
 * https://oeis.org/A213680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Recurrence:  a(0) = 6
 *               a(n) = 2*a(n-1)^2 / 3 – 3   (n ≥ 1)
 * (The division is exact for every n.)
 */
#include <stdint.h>  /* for uint64_t used by idx_t */

/* Compute a(n) for any n ≥ 0. */
seq_t a(idx_t n)
{
    seq_t value = (seq_t)6;          /* a(0) */
    if (n == 0) {
        return value;
    }
    for (idx_t i = 1; i <= n; ++i) {
        /* Compute 2*value^2 / 3 – 3 using only integer operators. */
        /* Order of operations is safe; division is exact. */
        value = value * value;          /* value^2   */
        value = value * (seq_t)2;       /* 2*value^2 */
        value = value / (seq_t)3;       /* /3        */
        value -= (seq_t)3;              /* –3        */
    }
    return value;
}
