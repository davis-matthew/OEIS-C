/*
 * A080333  Partial sums of A080278.
 * https://oeis.org/A080333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n + 3*a(floor(n/3)),  a(0) = 0
 * (This is equivalent to the stated summation formula)
 */
#include <stddef.h>

static seq_t a_rec(idx_t n)
{
    if (n == 0)
        return 0;
    /* cast n to seq_t to avoid mixing unsigned and integer types */
    return (seq_t)n + 3 * a_rec(n / 3);
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1, but a(0) is needed in the
     * recurrence; returning 0 for n==0 is harmless.
     */
    return a_rec(n);
}
