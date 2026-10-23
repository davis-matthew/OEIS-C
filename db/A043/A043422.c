/*
 * A043422  Numbers having two 0's in base 8.
 * https://oeis.org/A043422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer whose base‑8 representation
 *            contains exactly two zeros
 * Range:     u8 n <= 3, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* return true iff x written in base 8 has exactly two zero digits */
static bool has_two_zeros(idx_t x)
{
    seq_t v = x;      /* cast to seq_t for arithmetic */
    int zeros = 0;
    while (v > 0) {
        if ((v % 8) == 0) {
            ++zeros;
            if (zeros > 2) return false;
        }
        v /= 8;
    }
    return zeros == 2;
}

seq_t a(idx_t n)
{
    /* n ≥ 1 */
    idx_t cur = 0;
    while (1) {
        ++cur;
        if (has_two_zeros(cur)) {
            if (--n == 0)
                return (seq_t)cur;
        }
    }
}
