/*
 * A152263  a(n) = ((8 + sqrt(6))^n + (8 - sqrt(6))^n)/2.
 * https://oeis.org/A152263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((8 + sqrt(6))^n + (8 - sqrt(6))^n)/2 Recurrence: a(0) =
 *            1, a(1) = 8, a(n) = 16*a(n-1) - 58*a(n-2) for n > 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)8;

    /* Iteratively compute the sequence using the recurrence.
     * Only the last two terms are needed, so the algorithm
     * uses O(1) memory and runs in O(n) time with ordinary
     * integer arithmetic.  The constants 16 and 58 are
     * converted to seq_t and the operations use long
     * integer arithmetic that is exact for the given type. */
    seq_t a_nm2 = (seq_t)1;   /* a(0) */
    seq_t a_nm1 = (seq_t)8;   /* a(1) */
    seq_t a_n   = 0;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t term1 = a_nm1 * (seq_t)16;   /* a(n-1) * 16 */
        seq_t term2 = a_nm2 * (seq_t)58;   /* a(n-2) * 58 */
        a_n = term1 - term2;              /* a(n) = term1 - term2 */

        /* Shift the window forward */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
