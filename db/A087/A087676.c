/*
 * A087676  Consider recurrence b(0) = n/3, b(n) = b(0)*floor(b(n-1));
 *          sequence gives number of steps to reach an integer, or -1 if no
 *          integer is ever reached.
 * https://oeis.org/A087676
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the number of iterations k (starting from 0) needed for *
 *            the sequence b defined by * * b0 = n/3 * b(k+1) = b0 * floor(bk)
 *            * * to reach an integer value. If this never occurs, the
 *            function returns * -1. The index n >= 6 is used as in the OEIS
 *            entry
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 110, u64 n <= 110,
 *            u128 n <= 110, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* Handle the trivial integer case */
    if ((n % 3) == 0) {
        return (seq_t)0;
    }

    /* f represents floor(bk) at each step */
    seq_t f = (seq_t)(n / 3);          /* floor(n/3) */
    idx_t  steps = 1;                  /* we have just computed b1 from b0 */

    /* Iteratively apply the recurrence until n*f is divisible by 3 */
    while ( ((seq_t)n * f) % 3 != 0 ) {
        f = ((seq_t)n * f) / 3;        /* floor( n*f/3 ) */
        ++steps;
        /* safeguard against potential infinite loops (should not occur
           for n >= 2 according to the conjecture) */
        if (steps > 1 << 20) {
            return (seq_t)-1;
        }
    }

    return (seq_t)steps;
}
