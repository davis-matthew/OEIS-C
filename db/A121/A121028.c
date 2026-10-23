/*
 * A121028  Multiples of 8 containing an 8 in their decimal representation.
 * https://oeis.org/A121028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n-th multiple of 8 that contains at least one digit 8
 * Range:     u8 n <= 9, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* check whether a decimal representation of x contains the digit 8 */
static bool has8(seq_t x)
{
    while (x != (seq_t)0) {
        if (x % (seq_t)10 == (seq_t)8)
            return true;
        x /= (seq_t)10;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* n is 1-based */
    idx_t count = 0;
    idx_t k = 1;

    for (;; ++k) {
        seq_t m = (seq_t)8 * (seq_t)k; /* m is the k-th multiple of 8 */
        if (has8(m)) {
            ++count;
            if (count == n)
                return m;
        }
    }
}
