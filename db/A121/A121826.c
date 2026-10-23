/*
 * A121826  a(n) = Product_{k=1..n} D(k), where D() are the doublets, A020338.
 * https://oeis.org/A121826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   a(n) = ∏_{k=1}^n (k * 10^{digits(k)} + k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   digit_count, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"
#include "../utils/digit_count.h"

seq_t a(idx_t n) {
    /* Start with product 1 */
    seq_t prod = (seq_t)1;

    for (idx_t k = 1; k <= n; ++k) {
        /* Number of decimal digits in k */
        idx_t d = digit_count(k);

        /* D(k) = k * 10^d + k = k * (10^d + 1) */
        seq_t factor = pow10(d) + (seq_t)1;
        seq_t dk = (seq_t)k * factor;

        /* Multiply into running product */
        prod *= dk;
    }
    return prod;
}
