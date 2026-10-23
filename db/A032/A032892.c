/*
 * A032892  Numbers whose base-4 representation Sum_{i=0..m} d(i)*4^i has d(0)
 *          >= d(1) <= d(2) >= ...
 * https://oeis.org/A032892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th base‑4 alternating‑digit number (digits d0 >= d1 <=
 *            d2 >= d3 <= …)
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   alt_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/alt_digits.h"

/* Return the n‑th term of OEIS A032892. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t candidate = 0;              /* start from 1, see loop */

    while (count < n)
    {
        candidate++;                  /* check next integer */
        if (alt_digits(candidate))
            ++count;                 /* valid candidate found */
    }
    return candidate;
}
