/*
 * A045302  Numbers whose base-5 representation contains exactly two 3's and
 *          two 4's.
 * https://oeis.org/A045302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose base‑5 representation contains exactly two 3’s and
 *            two 4’s
 * Range:     u8 none, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   has_two3two4_ul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/has_two3two4_ul.h"

/* a(n) 1‑based index function */
seq_t a(idx_t n)
{
    /* We generate terms on demand; up to 10000 terms are needed and fit in 64‑bit integer. */
    uint64_t val = 1;
    size_t count = 0;

    while (count < n)
    {
        if (has_two3two4_ul(val))
        {
            count++;
            if (count == n)
                return (seq_t)val;  /* convert to the requested seq_t type */
        }
        val++;
    }

    /* Should never reach here */
    return (seq_t)0;
}
