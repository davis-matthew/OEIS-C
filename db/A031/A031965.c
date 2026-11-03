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
 * Validated: standard, pass, 61 terms through n = 2907 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return true if v has exactly four distinct digits in base 6. */
static bool has_four_distinct(seq_t v)
{
    if (v == 0)          /* base‑6 "0" has only one distinct digit */
        return false;

    unsigned mask = 0;
    seq_t t = v;
    while (t > 0) {
        unsigned d = (unsigned)(t % 6);
        mask |= 1U << d;
        t /= 6;
    }

    unsigned bits = 0;
    for (unsigned i = 0; i < 6; ++i)
        if (mask & (1U << i))
            ++bits;

    return bits == 4;
}

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
