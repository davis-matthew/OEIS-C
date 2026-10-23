/*
 * A178829  Partial sums of (1/2)*floor(7^n/10).
 * https://oeis.org/A178829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A178829 – partial sums of (1/2)*floor(7^n/10)
   Formula: a(n) = floor((7*7^n - 30*n - 7)/60)/2
   Recurrence (valid for n > 4):
      a(1)=0, a(2)=2, a(3)=19, a(4)=139
      a(n) = a(n-4) + 20*7^(n-3) - 1
*/
#include <stdint.h>  /* safe to include for uint64_t, no harm */

seq_t a(idx_t n)
{
    /* handle initial terms directly */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)19;
    if (n == 4) return (seq_t)139;

    /* iteratively build the sequence from 5 up to n using the recurrence */
    seq_t a0 = (seq_t)0;   /* a(i-4)  */
    seq_t a1 = (seq_t)2;   /* a(i-3)  */
    seq_t a2 = (seq_t)19;  /* a(i-2)  */
    seq_t a3 = (seq_t)139; /* a(i-1)  */

    /* 7^(i-3) for i = 5 starts as 7^2 = 49 */
    seq_t pow7 = (seq_t)49;

    for (idx_t i = 5; i <= n; ++i) {
        /* term = 20 * 7^(i-3) - 1 */
        seq_t term = (seq_t)20 * pow7 - (seq_t)1;

        /* new value a(i) = a(i-4) + term */
        seq_t new_val = a0 + term;

        /* rotate sliding window of the last four values */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = new_val;

        /* advance the power of 7 for the next step */
        pow7 = pow7 * (seq_t)7;
    }

    /* after the loop a3 holds a(n) */
    return a3;
}
