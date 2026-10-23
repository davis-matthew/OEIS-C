/*
 * A037563  Base 6 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,2,1.
 * https://oeis.org/A037563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 6*a(n-1) + digit_n where digit_n repeats (2,2,1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h> /* for BOOL if needed by build */

seq_t a(idx_t n)
{
    /* pattern of base-6 digits: 2,2,1 */
    static const unsigned int pat[3] = {2, 2, 1};
    seq_t val = 0;
    unsigned int idx = 0;          /* current position in pattern */
    for (idx_t i = 0; i < n; ++i) {
        val = val * 6 + pat[idx];
        idx = (idx + 1 == 3) ? 0 : idx + 1;
    }
    return val;
}
