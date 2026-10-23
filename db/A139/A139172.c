/*
 * A139172  a(n) = n!/2 - 1.
 * https://oeis.org/A139172
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(a(n-1) + 1) - 1, a(2) = 0
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Computes the OEIS sequence A139172.  For n < 2 the value is undefined
   (returns 0).  The sequence is defined for n ≥ 2. */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    /* a(2) = 0 */
    if (n == 2)
        return 0;

    /* Iterate using the recurrence */
    seq_t prev = 0;                /* holds a(k-1), starting with a(2) */
    idx_t i;
    for (i = 3; i <= n; ++i) {
        /* temp = (prev + 1) * i - 1 */
        seq_t temp = prev + 1;
        temp = temp * (seq_t)i;
        temp -= 1;
        prev = temp;
    }
    return prev;
}
