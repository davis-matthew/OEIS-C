/*
 * A173225  a(n) is the smallest nonnegative m such that average digit of m^2
 *          is n.
 * https://oeis.org/A173225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Validated: full, pass, 10 terms through n = 9 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the smallest m such that the average decimal digit of m² equals n
   (average digit = (sum of digits of m²)/(number of digits of m²)).
   Sequence defined only for 0 ≤ n ≤ 9; for n > 9 it returns 0. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n) {
    /* n > 9 is impossible, return 0 */
    if (n > 9) return 0;

    for (idx_t m = 0; ; ++m) {
        seq_t mm   = (seq_t)m;
        seq_t sq   = seq_mul(mm, mm);

        /* compute digit sum and digit count of sq */
        seq_t sum   = 0;
        idx_t  count = 0;

        if (sq == 0) {
            /* special case: 0 has one digit 0 */
            count = 1;
            sum   = 0;
        } else {
            for (seq_t t = sq; t != 0; t /= 10) {
                seq_t d = t % 10;
                sum += d;
                ++count;
            }
        }

        /* check if average digit equals n */
        seq_t target = (seq_t)n * (seq_t)count;
        if (sum == target)
            return mm;
    }
}
