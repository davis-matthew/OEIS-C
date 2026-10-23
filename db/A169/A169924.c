/*
 * A169924  Values of n >= 0 such that 4*n-40+360/(n+9) is an integer.
 * https://oeis.org/A169924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 17, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A169924: a(n) is the nth integer n ≥ 0 for which (n+9) divides 360.
   Enumerates all divisors d of 360 with d ≥ 9, returns n = d‑9. */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* numbers n with (n+9)|360 → n = d‑9 for divisors d of 360 (d ≥ 9) */
    seq_t values[17];
    size_t cnt = 0;
    for (int d = 9; d <= 360; ++d) {
        if (360 % d == 0) {
            values[cnt++] = (seq_t)d - 9;
        }
    }
    if (n == 0 || (size_t)n > cnt)
        return 0;
    return values[(size_t)(n - 1)];
}
