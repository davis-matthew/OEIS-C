/*
 * A059116  The sequence lambda(4,n), where lambda is defined in A055203.
 *          Number of ways of placing n identifiable positive intervals with a
 *          total of exactly four starting and/or finishing points.
 * https://oeis.org/A059116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^n - 4*3^n + 6 for n > 1, a(0) = 0, a(1) = 0
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A059116 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Handle the offset terms directly */
    if (n <= 1)
        return 0;

    /* Compute 6^n and 3^n iteratively.  All operations use the native seq_t type
       so that the function works for the arbitrarily‑precise build as well as
       the fixed‑width builds.  Multiplications do not overflow for all n
       whose term fits in the target seq_t type.  */
    seq_t p6 = 1;
    seq_t p3 = 1;
    for (idx_t i = 1; i <= n; ++i) {
        p6 *= 6;
        p3 *= 3;
    }

    /* 4*3^n can be written as (3^n) << 2 to avoid intermediate overflow.   */
    seq_t res = p6 - (p3 << 2) + 6;
    return res;
}
