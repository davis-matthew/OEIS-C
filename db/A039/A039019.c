/*
 * A039019  Numbers whose base-6 representation has the same number of 0's and
 *          4's.
 * https://oeis.org/A039019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth natural number such that the number of 0's equals the
 *            number of 4's in its base‑6 representation
 * Range:     u8 n <= 109, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline bool __same_zero_four(seq_t x)
{
    unsigned zeros = 0, fours = 0;
    while (x > 0) {
        unsigned digit = (unsigned)(x % 6);
        if (digit == 0) {
            ++zeros;
        } else if (digit == 4) {
            ++fours;
        }
        x /= 6;
    }
    return zeros == fours;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* not defined for n == 0 */
    seq_t count = 0;
    seq_t x = 1;
    while (1) {
        if (__same_zero_four(x)) {
            ++count;
            if (count == n) return x;
        }
        ++x;
    }
}
