/*
 * A159501  Numerator of Hermite(n, 8/13).
 * https://oeis.org/A159501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 6, i64 n <= 11, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A159501: Numerator of Hermite(n, 8/13)
 * Recurrence:  a(0)=1,  a(1)=16
 *             a(n+1)=16*a(n) - 338*n*a(n-1)   (for n>=1)
 * From the generating function exp(16*x-169*x^2)
 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)16;

    seq_t a_prevprev = (seq_t)1;  /* a(0) */
    seq_t a_prev     = (seq_t)16; /* a(1) */
    seq_t next       = 0;

    for (idx_t i = 1; i < n; ++i) {
        /* Compute a(i+1) = 16*a(i) - 338*i*a(i-1) */
        seq_t temp = a_prevprev * (seq_t)i * (seq_t)338; /* 338*i*a(i-1) */
        next = a_prev * (seq_t)16 - temp;               /* 16*a(i) - temp */
        a_prevprev = a_prev;
        a_prev = next;
    }
    return a_prev;
}
