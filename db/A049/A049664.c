/*
 * A049664  a(n) = (F(6*n+3) - 2)/32, where F = A000045 (the Fibonacci
 *          sequence).
 * https://oeis.org/A049664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1; a(n) = 18*a(n-1) - a(n-2) + 1 for n >= 2
 *            (recurrence from the OEIS entry)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>          /* for size_t, but not used directly */

seq_t a(idx_t n)
{
    /* Handle the trivial first two terms. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Iteratively compute the sequence from a(0) and a(1). */
    seq_t a0 = (seq_t)0;   /* a(n-2)   */
    seq_t a1 = (seq_t)1;   /* a(n-1)   */
    seq_t an;              /* a(n)     */

    for (idx_t i = 2; i <= n; ++i) {
        an = 18 * a1 - a0 + 1;
        a0 = a1;
        a1 = an;
    }
    return a1;
}
