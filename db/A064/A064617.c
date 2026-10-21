/*
 * A064617  a(n) = (10^n - 1)*(80/81) + n/9.
 * https://oeis.org/A064617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 10*a(n-1) + 10 - n,     a(1) = 9
 * (equivalent to a(n) = (10^n - 1)*(80/81) + n/9)
 *
 * This implementation evaluates the recurrence iteratively.
 * No memoisation or dynamic memory is used.
 */
#include <stddef.h>   /* for NULL if needed */

seq_t a(idx_t n)
{
    seq_t val;
    idx_t i;

    /* Sequence is defined for n ≥ 1. For 0 or negative indices return 0. */
    if (n == 0)
        return (seq_t)0;

    /* Initialise first term. */
    val = (seq_t)9;

    /* Compute subsequent terms iteratively. */
    for (i = 2; i <= n; ++i) {
        val = val * (seq_t)10 + (seq_t)10;  /* val = 10*val + 10 */
        val -= i;                           /* val = val - i       */
    }

    return val;
}
