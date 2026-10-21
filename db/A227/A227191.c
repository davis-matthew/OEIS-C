/*
 * A227191  a(n) = n minus (product of nonzero digits in factorial base
 *          representation of n).
 * https://oeis.org/A227191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n - product of nonzero digits in the factorial base
 *            representation of n
 * Range:     u8 n <= 263, u16 n <= 5040, u32 n <= 5040, u64 n <= 5040,
 *            u128 n <= 5040, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is indexed from 1.  For n==0 we return 0, but calls with n<1 are undefined. */
    if (n == 0) {
        return 0;
    }

    idx_t remaining = n;
    seq_t prod = 1;

    /* Process each factorial place, starting from the highest one. */
    while (remaining > 0) {
        /* Find the largest factorial m! that does not exceed remaining. */
        idx_t fact = 1;    /* 1! */
        idx_t m = 1;       /* current factorial index */

        while (1) {
            /* If the next factorial (m+1)! would overflow or exceed remaining, stop. */
            if (fact > remaining / (m + 1)) {
                break;
            }
            ++m;
            fact *= m;     /* advance to (m)! */
        }

        /* The digit for this factorial place. */
        idx_t digit = remaining / fact;
        remaining %= fact;

        if (digit != 0) {
            prod *= (seq_t)digit;   /* multiply only non‑zero digits */
        }
    }

    return (seq_t)n - prod;
}
