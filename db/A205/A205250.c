/*
 * A205250  Number of (n+1) X 4 0..1 arrays with the number of clockwise edge
 *          increases in every 2 X 2 subblock the same.
 * https://oeis.org/A205250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 24*a(n-2) + 21*a(n-3) - 6*a(n-4) Initial
 *            values: a(1)=112, a(2)=752, a(3)=5212, a(4)=36304
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle index 0 gracefully (not defined for this sequence) */
    if (n == 0) return 0;

    /* Return the known initial values directly */
    if (n == 1) return (seq_t)112;
    if (n == 2) return (seq_t)752;
    if (n == 3) return (seq_t)5212;
    if (n == 4) return (seq_t)36304;

    /* Iteratively compute using a 4‑term sliding window */
    seq_t a1 = 112;   /* a(n-4)  */
    seq_t a2 = 752;   /* a(n-3)  */
    seq_t a3 = 5212;  /* a(n-2)  */
    seq_t a4 = 36304; /* a(n-1)  */
    seq_t ai;          /* current a(n) */

    for (idx_t i = 5; i <= n; ++i) {
        ai = ( (seq_t)10 * a4 ) - ( (seq_t)24 * a3 )
            + ( (seq_t)21 * a2 ) - ( (seq_t)6 * a1 );

        /* Shift the window forward */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = ai;
    }

    return a4;
}
