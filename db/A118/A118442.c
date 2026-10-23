/*
 * A118442  Column 0 of triangle A118441, which is the matrix log of triangle
 *          A118435.
 * https://oeis.org/A118442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 12, i32 n <= 27, i64 n <= 58, i128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A118442
 *
 *  Formula: a(n) = (-1)^floor(n/2) * A001787(n)
 *  Recurrence: a(0)=0, a(1)=1, a(2)=-4, a(3)=-12,
 *              a(n) = -8*a(n-2) - 16*a(n-4)   for n >= 4
 */

#include <stdint.h>

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    /* Handle the small indices directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)-4;
    if (n == 3) return (seq_t)-12;

    /* Roll four recent terms:
     *   a0 = a(i-4), a1 = a(i-3), a2 = a(i-2), a3 = a(i-1)
     * Start with i = 4. */
    seq_t a0 = (seq_t)0;   /* a(0) */
    seq_t a1 = (seq_t)1;   /* a(1) */
    seq_t a2 = (seq_t)-4;  /* a(2) */
    seq_t a3 = (seq_t)-12; /* a(3) */

    /* Iterate up to n, computing each new term from the recurrence. */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t cur = -( (seq_t)8 * a2 ) - ( (seq_t)16 * a0 ); /* a(i) */
        /* Shift the history for the next iteration. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = cur;
    }

    return a3; /* a(n) is in a3 after the loop */
}
