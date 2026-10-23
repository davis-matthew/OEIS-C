/*
 * A021369  Decimal expansion of 1/365.
 * https://oeis.org/A021369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/365) - 10*floor(10^n/365) (the nth decimal
 *            digit of 1/365). This computes the digit by repeatedly
 *            multiplying the remainder by 10 and dividing by 365
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    uint64_t rem = 1ULL;    /* current remainder of 1/365 after the decimal point */
    for (idx_t i = 0; i <= n; ++i) {
        rem *= 10ULL;
        seq_t d = (seq_t)(rem / 365ULL);
        rem %= 365ULL;
        if (i == n) return d;
    }
    return 0;   /* unreachable */
}
