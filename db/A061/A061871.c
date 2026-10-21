/*
 * A061871  |First digit - second digit + third digit - fourth digit ...| = 2.
 * https://oeis.org/A061871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 7164, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_alt_sum2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Alternating-digit-sum–2 sequence:
   a(n) is the nth positive integer whose decimal digits d1,d2,…
   satisfy |d1 - d2 + d3 - d4 + …| = 2.
   No formula is supplied; the definition is evaluated by brute‑force search. */

#include <stdint.h>
#include "../utils/is_alt_sum2.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* Sequence is undefined for n < 1 */
    idx_t count = 0;
    idx_t num = 1;
    while (1) {
        if (is_alt_sum2(num)) {
            ++count;
            if (count == n) return (seq_t)num;
        }
        ++num;
    }
}
