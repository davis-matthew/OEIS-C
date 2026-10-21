/*
 * A219390  Numbers k such that 14*k+1 is a square.
 * https://oeis.org/A219390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (14*n*(n-1)+5*(-1)^n*(2*n-1)+1)/4 +1
 * Range:     u8 n <= 9, u16 n <= 137, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Convert the index to a signed 64‑bit value for intermediate arithmetic. */
    int64_t n_ll = (int64_t)n;

    /* Compute the numerator of the formula. */
    int64_t termA = 14LL * n_ll * (n_ll - 1);
    int64_t sign = (n_ll % 2 == 0) ? 1 : -1;
    int64_t termB = 5LL * sign * (2 * n_ll - 1);
    int64_t numerator = termA + termB + 1;

    /* Integer division; the numerator is always divisible by 4. */
    int64_t result = numerator / 4 + 1;

    return (seq_t)result;
}
