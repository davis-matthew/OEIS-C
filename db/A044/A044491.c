/*
 * A044491  Numbers n such that string 1,2 occurs in the base 6 representation
 *          of n but not of n+1.
 * https://oeis.org/A044491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   contains12
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044491:
 *  Numbers n such that the base‑6 representation of n contains the
 *  substring 1,2 but the base‑6 representation of n+1 does NOT.
 */
#include <stdbool.h>
#include "../utils/contains12.h"

seq_t a(idx_t n)
{
    if (n == 0)     /* undefined for n = 0, but harmless */
        return 0;

    idx_t count = 0;
    for (idx_t cand = 1; ; ++cand) {
        if (contains12((seq_t)cand) && !contains12((seq_t)(cand + 1))) {
            ++count;
            if (count == n)
                return (seq_t)cand;
        }
    }
}
