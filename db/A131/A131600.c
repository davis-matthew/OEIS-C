/*
 * A131600  Number of different configurations of an n-block of a shift space
 *          with k symbols where each symbol but the first must appear
 *          isolated and separated from others by a block of length at least m
 *          made of first symbols. Here k=19 and m=2.
 * https://oeis.org/A131600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 18, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + 18*a(n-3); a(1) = 19, a(2) = 37, a(3) = 55 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                  /* below offset, behavior undefined */
    if (n == 1)
        return (seq_t)19;
    if (n == 2)
        return (seq_t)37;
    if (n == 3)
        return (seq_t)55;

    /* sliding window of the last three terms */
    seq_t a1 = (seq_t)19;                 /* a(n-3) */
    seq_t a2 = (seq_t)37;                 /* a(n-2) */
    seq_t a3 = (seq_t)55;                 /* a(n-1) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t ai = a3 + (seq_t)18 * a1;   /* a(n) = a(n-1) + 18*a(n-3) */
        a1 = a2;                         /* shift window forward */
        a2 = a3;
        a3 = ai;
    }
    return a3;                            /* a(n) */
}
