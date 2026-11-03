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
 * Validated: partial, pass, 44 terms through n = 44 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Check if a uint64_t value has exactly two digits 3 and two digits 4 in base 5 */
static bool has_two3two4_ul(uint64_t x)
{
    int cnt3 = 0, cnt4 = 0;
    while (x > 0)
    {
        int d = (int)(x % 5);
        if (d == 3) cnt3++;
        else if (d == 4) cnt4++;
        if (cnt3 > 2 || cnt4 > 2) return false;
        x /= 5;
    }
    return (cnt3 == 2 && cnt4 == 2);
}

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
