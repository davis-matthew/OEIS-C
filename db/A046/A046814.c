/*
 * A046814  Row sums of triangle A046527.
 * https://oeis.org/A046814
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A046814: Row sums of triangle A046527
 * Recurrence: a(0)=1, a(1)=2,
 *             a(n) = 5*a(n-1) - 3*C(n)/(2*n-1)  for n >= 2
 * where C(n) is the nth Catalan number.
 * The recurrence is derived from the D‑finite recurrence in the OEIS entry.
 * All divisions are exact for integer arguments.
 */

#include <stdint.h>  /* for uint64_t, if needed */

seq_t a(idx_t n)
{
    /* Handle trivial indices */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* Variables for the recurrence */
    seq_t prev   = (seq_t)2;   /* a(1) */
    seq_t cat    = (seq_t)1;   /* Catalan C(1) */

    /* Compute successive values up to a(n) */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t i_seq = (seq_t)i;                     /* cast index to seq_t */
        /* Update Catalan number: C(i) = C(i-1) * (4*i-2) / (i+1) */
        seq_t mult = cat * ((seq_t)4 * i_seq - (seq_t)2);
        seq_t den  = i_seq + (seq_t)1;
        cat = mult / den;                           /* exact division */

        /* Compute a(i) using the recurrence */
        seq_t sub  = (seq_t)3 * cat / ( (seq_t)2 * i_seq - (seq_t)1 );
        seq_t cur  = (seq_t)5 * prev - sub;
        prev = cur;                                 /* shift for next iteration */
    }

    return prev;   /* a(n) */
}
