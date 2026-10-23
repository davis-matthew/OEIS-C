/*
 * A043711  Numbers whose base-8 representation has an odd number of runs.
 * https://oeis.org/A043711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the n-th positive integer whose base‑8 representation
 * has an odd number of runs.  Runs are maximal blocks of equal digits,
 * so this is equivalent to numbers whose digits change an even number
 * of times when read from most significant to least significant. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* The OEIS offset is 1, so return 0 for n==0 (undefined case). */
    if (n == 0)
        return 0;

    seq_t val = 1;          /* candidate integer */
    idx_t  found = 0;       /* how many qualifying numbers found */

    while (1) {
        seq_t tmp = val;
        unsigned int changes = 0;
        unsigned int prev_digit = 0;
        bool first_digit = true;

        /* Count changes (adjacent unequal digits) in base‑8 representation. */
        while (tmp > 0) {
            unsigned int digit = (unsigned int)(tmp % 8);
            if (first_digit) {
                /* First digit: no previous digit to compare. */
                first_digit = false;
            } else if (digit != prev_digit) {
                ++changes;
            }
            prev_digit = digit;
            tmp /= 8;
        }

        /* Odd number of runs ⇔ even number of changes. */
        if ((changes & 1) == 0) {
            ++found;
            if (found == n)
                return val;
        }

        ++val;   /* examine next integer */
    }
}
