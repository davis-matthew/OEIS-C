/*
 * A042198  Numerators of continued fraction convergents to sqrt(624).
 * https://oeis.org/A042198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 9, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 50*a(n-2) - a(n-4)
   initial values: a(0)=24, a(1)=25, a(2)=1224, a(3)=1249
   Computes the numerator of the nth convergent to sqrt(624). */
seq_t a(idx_t n)
{
    /* handle first four indices directly */
    if (n == 0) return (seq_t)24;
    if (n == 1) return (seq_t)25;
    if (n == 2) return (seq_t)1224;
    if (n == 3) return (seq_t)1249;

    /* values for the last four computed terms
       a0 = a(i-4), a1 = a(i-3), a2 = a(i-2), a3 = a(i-1)  */
    seq_t a0 = (seq_t)24;
    seq_t a1 = (seq_t)25;
    seq_t a2 = (seq_t)1224;
    seq_t a3 = (seq_t)1249;
    seq_t next = a3;  /* placeholder */

    for (idx_t i = 4; i <= n; ++i) {
        /* a(i) = 50*a(i-2) - a(i-4) */
        next = a2 * (seq_t)50 - a0;

        /* shift the window for the next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return next;
}
