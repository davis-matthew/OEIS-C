/*
 * A219259  Numbers k such that 25*k+1 is a square.
 * https://oeis.org/A219259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (50*n*(n-1)+21*(-1)^n*(2*n-1)+5)/8+2
 * Range:     u8 n <= 7, u16 n <= 103, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Convert index to signed 64‑bit for intermediate arithmetic */
    int64_t nn = (int64_t)n;
    /* Compute the numerator */
    int64_t val = 50LL * nn * (nn - 1);
    /* Add or subtract the parity term */
    if ((nn & 1) == 0) {            /* n even → (+) */
        val += 21LL * (2 * nn - 1);
    } else {                        /* n odd  → (−) */
        val -= 21LL * (2 * nn - 1);
    }
    val += 5;                       /* constant term */
    /* Division by 8 (exact) and final offset */
    seq_t result = (seq_t)(val / 8 + 2);
    return result;
}
