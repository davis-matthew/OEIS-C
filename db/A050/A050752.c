/*
 * A050752  Fifth powers containing no pair of consecutive equal digits.
 * https://oeis.org/A050752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth fifth power of an integer whose decimal
 *            representation has no consecutive equal digits. Computed by a
 *            brute‑force search
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   no_consecutive_equal_digits, pow5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/no_consecutive_equal_digits.h"
#include "../utils/pow5.h"

/* Return the nth member of sequence A050752 (offset 0). */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;               /* 0^5 = 0, first term */

    idx_t found = 0;                           /* count of qualifying powers seen so far */
    for (idx_t k = 1;; ++k) {
        seq_t power = pow5((seq_t)k);          /* compute k^5 */
        if (no_consecutive_equal_digits(power)) {
            ++found;
            if (found == n) return power;     /* nth qualifying fifth power found */
        }
    }
}
