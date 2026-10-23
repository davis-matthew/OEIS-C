/*
 * A044875  Numbers having, in base 4, (sum of even run lengths)=(sum of odd
 *          run lengths).
 * https://oeis.org/A044875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer whose base‑4 representation has
 *            equal total lengths of runs of even length and runs of odd
 *            length
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   equal_run_parity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/equal_run_parity.h"

seq_t a(idx_t n)
{
    seq_t candidate = 0;
    idx_t found = 0;
    while (found < n) {
        candidate++;
        if (equal_run_parity(candidate))
            found++;
    }
    return candidate;
}
