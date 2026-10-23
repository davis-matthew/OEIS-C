/*
 * A025783  Expansion of 1/((1-x)(1-x^6)(1-x^11)).
 * https://oeis.org/A025783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{j=0}^{floor(n/6)} (floor((n-6j)/11) + 1) This counts
 *            the number of non‑negative integer solutions to i + 6*j + 11*k =
 *            n
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* loop over number of 6‑parts */
    for (idx_t j = 0; j <= n / 6; ++j) {
        idx_t rem = n - j * 6;          /* remaining after 6‑parts */
        seq_t term = 1 + (seq_t)(rem / 11); /* 1 + number of 11‑parts fit */
        sum += term;
    }
    return sum;
}
