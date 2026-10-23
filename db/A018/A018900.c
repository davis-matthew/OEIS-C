/*
 * A018900  Sums of two distinct powers of 2.
 * https://oeis.org/A018900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, tabl, look
 * Formula:   a(n) = 2^i + 2^j where i = ceil((sqrt(8n+1)-1)/2), j = n -
 *            i*(i-1)/2 - 1 (i ≥ 1, 0 ≤ j < i). This yields the nth smallest
 *            integer with exactly two 1‑bits in binary
 * Range:     u8 n <= 28, u16 n <= 120, u32 n <= 496, u64 n <= 2016,
 *            u128 n <= 8128, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* n is 1‑based; n ≥ 1 for valid requests */
    /* Find minimal i such that i*(i+1)/2 ≥ n */
    idx_t low = 1, high = 1;
    while ((((seq_t)high * ((seq_t)high + 1)) / 2) < n) {
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if ((((seq_t)mid * ((seq_t)mid + 1)) / 2) >= n) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    idx_t i = low;

    /* Number of pairs with i-1 as the first coordinate */
    seq_t prev_total = ((seq_t)(i - 1) * (seq_t)i) / 2;
    /* Position within the block for this i (1‑based) */
    seq_t pos = (seq_t)n - prev_total;
    /* Corresponding j (0‑based) */
    idx_t j = (idx_t)(pos - 1);

    return pow2(i) + pow2(j);
}
