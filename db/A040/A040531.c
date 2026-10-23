/*
 * A040531  Continued fraction for sqrt(555).
 * https://oeis.org/A040531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction partial quotients for sqrt(555) */

#include <stdint.h>

/* seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n)
{
    /* Handle first term (floor of sqrt(555)) */
    if (n == 0) {
        return (seq_t)23;          /* 23*23 = 529 < 555 < 576 = 24*24 */
    }

    /* Standard continued‑fraction expansion for a non‑square integer
       D = 555.  We use 64‑bit unsigned arithmetic which is safe
       because intermediate values are bounded by D. */
    const uint64_t D = 555;
    const uint64_t a0 = 23;          /* floor(sqrt(D)) */

    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t a_val = a0;

    /* Compute partial quotients up to index n */
    for (idx_t i = 1; i <= n; ++i) {
        /* Next m and d */
        m = d * a_val - m;
        d = (D - m * m) / d;
        /* Next a */
        a_val = (a0 + m) / d;
    }

    return (seq_t)a_val;
}
