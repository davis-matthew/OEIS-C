/*
 * A037975  Maximal base 3 run length is 4.
 * https://oeis.org/A037975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   has_run4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose base‑3 representation has a maximal run of 4 identical digits.
   The function enumerates integers and returns the nth such number. */
#include <stdbool.h>
#include "../utils/has_run4.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* offset starts at 1 */
    seq_t count = 0;
    idx_t num = 0;
    while (1) {
        ++num;
        if (has_run4(num)) {
            ++count;
            if (count == n) return (seq_t)num;
        }
    }
}
