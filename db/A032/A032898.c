/*
 * A032898  Numbers whose base-10 representation Sum_{i=0..m} d(i)*10^i, d(m)
 *          > 0, has d(0) >= d(1) <= d(2) >= ...
 * https://oeis.org/A032898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer whose decimal digits d(0)…d(m) (with
 *            d(m)>0) satisfy the alternating inequalities d(0) >= d(1) <=
 *            d(2) >= d(3) <= … (starting with “>=”)
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   valid_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/valid_digits.h"

/* Return the nth term of A032898 (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n=0, but safe return */

    idx_t target = (idx_t)n;
    idx_t count  = 0;
    seq_t x = 1;

    for (;;) {
        if (valid_digits(x)) {
            ++count;
            if (count == target) return x;
        }
        ++x;
    }
}
