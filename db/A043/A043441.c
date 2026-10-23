/*
 * A043441  Numbers having one 5 in base 8.
 * https://oeis.org/A043441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth integer whose base‑8 representation contains exactly
 *            one digit 5
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   has_one_five_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_one_five_seq.h"

seq_t a(idx_t n)
{
    idx_t seen = 0;       /* number of qualifying numbers found */
    seq_t candidate = 0;  /* current integer to test */

    while (1)
    {
        if (has_one_five_seq(candidate))
        {
            ++seen;
            if (seen == n)
                return candidate;
        }
        ++candidate;
    }
}
