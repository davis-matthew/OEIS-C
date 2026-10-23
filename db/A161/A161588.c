/*
 * A161588  The list of the k values in the common solutions to the 2
 *          equations 11*k+1 = A^2, 15*k+1 = B^2.
 * https://oeis.org/A161588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+3) = 168*(a(n+2)-a(n+1)) + a(n) (OEIS A161588)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(1)=0, a(2)=13, a(3)=2184. */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the offset (OEIS starts at n = 1). */
    if (n == 1)
        return (seq_t)0;
    if (n == 2)
        return (seq_t)13;
    if (n == 3)
        return (seq_t)2184;

    /* Use three preceding terms to generate the sequence. */
    seq_t a1 = (seq_t)0;          /* a(n-3)   */
    seq_t a2 = (seq_t)13;         /* a(n-2)   */
    seq_t a3 = (seq_t)2184;       /* a(n-1)   */
    seq_t an = a3;                /* placeholder for a(n) */

    for (idx_t i = 4; i <= n; ++i) {
        an = (seq_t)168 * (a3 - a2) + a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }

    return an;
}
