/*
 * A165933  Least integer, k, whose value is n in A165413.
 * https://oeis.org/A165933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   ones_mask, shift_left_by
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ones_mask.h"
#include "../utils/shift_left_by.h"
/* Sequence A165933: binary pattern with alternating runs of 1s and 0s,
   starting with a single 1, followed by n zeros, then 2 ones, (n-1) zeros,
   3 ones, … until a total of n runs have been produced. */

seq_t a(idx_t n) {
    if (n < 1) return 0;   /* undefined for n < 1 */
    seq_t result = 1;       /* first run: single 1 */

    for (idx_t r = 2; r <= n; ++r) {
        if ((r & 1U) == 0) {           /* even run: zeros */
            idx_t zeros_len = n - (r / 2) + 1;
            shift_left_by(&result, zeros_len);
        } else {                        /* odd run (>=3): ones */
            idx_t ones_len = (r + 1) / 2;
            shift_left_by(&result, ones_len);
            result |= ones_mask(ones_len);
        }
    }
    return result;
}
