/*
 * A111896  Number of numbers m <= n such that 6 equals the second digit after
 *          decimal point of square root of n in decimal representation.
 * https://oeis.org/A111896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of m ≤ n such that the second decimal digit of √m is 6.
   a(n) = |{ m ∈ [1 .. n] : floor(√m · 100) mod 10 == 6 }|. */

#include <stdint.h>
#include "../utils/int_sqrt.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;          /* undefined below offset, return 0 */

    seq_t cnt = 0;                 /* prefix count */

    for (idx_t m = 1; m <= n; ++m) {
        /* compute floor(√m · 100) as floor(√(m * 10 000)) */
        idx_t prod = m * 10000ULL;   /* 10 000 fits in idx_t */
        idx_t s    = int_sqrt(prod); /* integer part of √(m * 10 000) */

        if ((s % 10) == 6)          /* second decimal digit == 6 */
            ++cnt;
    }

    return cnt;
}
