/*
 * A133017  Decimal expansion of the constant formed by concatenating the
 *          imperfect numbers.
 * https://oeis.org/A133017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  cons, easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_perfect
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A133017: Decimal expansion of the constant formed by concatenating
   all imperfect numbers (all positive integers except perfect numbers).  The
   function returns the n-th digit (0‑based) of that infinite concatenated
   string. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_perfect.h"

seq_t a(idx_t n)
{
    idx_t idx = 0;      /* position in the concatenated digit stream   */
    uint64_t k = 1;     /* current integer to consider                */

    for (;;) {
        if (is_perfect(k)) {      /* skip perfect numbers */
            ++k;
            continue;
        }

        /* Extract digits of k into a temporary buffer, most significant
           digit first.  Since k ≤ 2^64−1, at most 20 decimal digits.  */
        uint8_t dig[20];
        int dlen = 0;
        uint64_t tmp = k;
        do {
            dig[dlen++] = (uint8_t)(tmp % 10);
            tmp /= 10;
        } while (tmp);

        /* Emit digits from most significant to least.  The dlen‑1 index
           corresponds to the most significant digit.  */
        int i;
        for (i = dlen - 1; i >= 0; --i) {
            if (idx == n) {
                return (seq_t)dig[i];
            }
            ++idx;
        }
        ++k;
    }
}
