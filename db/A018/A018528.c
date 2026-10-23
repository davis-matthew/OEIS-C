/*
 * A018528  Divisors of 576.
 * https://oeis.org/A018528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the nth divisor of 576 (sorted in increasing order)
 * Range:     u8 n <= 2, u16 n <= 21, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h> /* required for bool type, not used directly here */

seq_t a(idx_t n)
{
    /* Offset is 1: a(1) = 1.  If n is outside the range of 21 divisors,
       return 0.  This matches OEIS convention that the sequence is
       undefined beyond its finite list. */
    if (n == 0) {
        return 0;
    }
    seq_t count = 0;
    for (idx_t i = 1; i <= 576; ++i) {
        if (((seq_t)576) % (seq_t)i == (seq_t)0) {
            ++count;
            if (count == n) {
                return (seq_t)i;
            }
        }
    }
    return 0;
}
