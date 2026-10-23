/*
 * A138988  a(n) is the Frobenius number for 8 successive numbers n+1, n+2,
 *          ..., n+8.
 * https://oeis.org/A138988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*floor((n-1)/7 + 1) - 1 (largest non‑representable
 *            number for coin denominations n+1,…,n+8)
 * Range:     u8 n <= 41, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* compute a = n+1  (coin start)           */
    seq_t a_start = (seq_t)(n + 1);
    /* t = floor((n-1)/7)                         */
    seq_t t = (seq_t)((n - 1) / 7);
    return a_start * (t + 1) - 1;
}
