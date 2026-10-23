/*
 * A165232  a(0)=1, a(1)=9, a(n) = 18*a(n-1) - 9*a(n-2) for n>1.
 * https://oeis.org/A165232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*a(n-1) - 9*a(n-2), a(0)=1, a(1)=9
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)9;

    /* Iteratively compute the sequence up to n. */
    seq_t a0 = (seq_t)1;          /* a(0) */
    seq_t a1 = (seq_t)9;          /* a(1) */
    seq_t an;                     /* placeholder for current term */

    for (idx_t i = 2; i <= n; ++i) {
        an = (seq_t)18 * a1 - (seq_t)9 * a0;  /* recurrence */
        a0 = a1;   /* shift for next iteration */
        a1 = an;
    }
    return an;
}
