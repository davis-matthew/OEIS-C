/*
 * A067448  Smallest n-th power whose decimal expansion starts with 7.
 * https://oeis.org/A067448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = smallest integer of the form b^n whose decimal expansion
 *            starts with 7
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 595 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <math.h>
#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    /* Constants for the fractional interval corresponding to a leading digit 7. */
    static const long double LOG7 = log10l((long double)7.0L);
    static const long double LOG8 = log10l((long double)8.0L);

    uint64_t base = 2;  /* start with the smallest non‑trivial base */

    while (1) {
        long double prod = (long double)n * log10l((long double)base);
        long double frac = prod - floorl(prod);

        /* Is the leading digit of base^n equal to 7? */
        if (frac >= LOG7 && frac < LOG8)
            break;

        ++base;
    }

    /* Return the exact value base^n.  pow_uint handles arbitrary‑precision arithmetic. */
    return ipow_u64_idx(base, n);
}
