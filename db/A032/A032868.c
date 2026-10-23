/*
 * A032868  Numbers whose base-5 representation Sum_{i=0..m} d(i)*5^i has d(m)
 *          >= d(m-1) <= d(m-2) >= ...
 * https://oeis.org/A032868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth positive integer whose base‑5 representation
 *            satisfies d(m) >= d(m-1) <= d(m-2) >= d(m-3) <= … (alternating
 *            inequalities)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   valid_alt_base5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/valid_alt_base5.h"

/* Entry point for the sequence.  Position n is 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for the given offset */

    idx_t count = 0;
    seq_t x = 1;
    for (;;) {
        if (valid_alt_base5(x)) {
            ++count;
            if (count == n) return x;
        }
        ++x;
    }
}
