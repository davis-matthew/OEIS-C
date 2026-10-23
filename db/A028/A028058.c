/*
 * A028058  Expansion of 1/((1-3x)(1-5x)(1-6x)(1-12x)).
 * https://oeis.org/A028058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A028058: Expansion of 1/((1-3x)(1-5x)(1-6x)(1-12x)).
   Recurrence: a(0)=1, a(1)=26, a(2)=445, a(3)=6410,
                a(n)=26*a(n-1)-231*a(n-2)+846*a(n-3)-1080*a(n-4)   (n≥4)
*/
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)26;
    if (n == 2) return (seq_t)445;
    if (n == 3) return (seq_t)6410;

    /* Sliding window for the last four values. */
    seq_t a0 = (seq_t)1;      /* a(n-4) */
    seq_t a1 = (seq_t)26;     /* a(n-3) */
    seq_t a2 = (seq_t)445;    /* a(n-2) */
    seq_t a3 = (seq_t)6410;   /* a(n-1) */

    for (idx_t i = 4; i <= n; ++i) {
        /* Compute the next term using the recurrence. */
        seq_t term = (seq_t)26 * a3;
        term -= (seq_t)231 * a2;
        term += (seq_t)846 * a1;
        term -= (seq_t)1080 * a0;

        /* Shift the window for the next iteration. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = term;
    }

    return a3;
}
