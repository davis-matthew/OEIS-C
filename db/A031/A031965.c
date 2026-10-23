/*
 * A031965  Numbers with exactly four distinct base-6 digits.
 * https://oeis.org/A031965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth integer whose base‑6 representation contains exactly
 *            four distinct digits
 * Range:     u8 n <= 12, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   has_four_distinct
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/has_four_distinct.h"

/* OEIS A031965: numbers with exactly four distinct base‑6 digits. */
seq_t a(idx_t n)
{
    idx_t found = 0;   /* number of qualifying integers seen so far */
    seq_t num = 0;     /* current integer candidate */

    while (found < n) {
        if (has_four_distinct(num)) {
            ++found;
            if (found == n)
                return num;
        }
        ++num;
    }
    /* Should never reach here for valid n >= 1. */
    return 0;
}
