/*
 * A045270  Numbers whose base-5 representation contains exactly two 2's and
 *          no 3's.
 * https://oeis.org/A045270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose base-5 representation contains exactly two 2's and no 3's */
#include <stdbool.h>

/* Helper: test whether x has exactly two digits 2 and no digit 3 in base‑5. */
static bool valid_base5(seq_t x)
{
    unsigned twos = 0;
    while (x > 0) {
        unsigned d = (unsigned)(x % 5);
        if (d == 3) return false;
        if (d == 2) {
            ++twos;
            if (twos > 2) return false;
        }
        x /= 5;
    }
    return twos == 2;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* out of range, but harmless */
    idx_t found = 0;
    for (seq_t x = 1; ; ++x) {
        if (valid_base5(x)) {
            ++found;
            if (found == n) return x;
        }
    }
}
