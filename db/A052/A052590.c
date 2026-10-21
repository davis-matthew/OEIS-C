/*
 * A052590  E.g.f. (1-x)/(1-4x+2x^2).
 * https://oeis.org/A052590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of A052590 using its linear recurrence.
   Recurrence: (2*n^2+6*n+4)*a(n) + (-4*n-8)*a(n+1) + a(n+2) = 0,
   with initial values a(0)=1, a(1)=3. */

/* Return the n-th term of OEIS sequence A052590. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)3;   /* a(1) */

    /* Compute successive terms up to a(n) using the recurrence. */
    for (idx_t i = 0; i < n-1; ++i) {   /* i corresponds to current n in the recurrence */
        seq_t coeff1 = (seq_t)(4*i + 8);                 /* (4*n+8) */
        seq_t coeff2 = (seq_t)(2*i*i + 6*i + 4);         /* (2*n^2+6*n+4) */
        seq_t a_next = coeff1 * a1 - coeff2 * a0;       /* a(n+2) */
        a0 = a1;
        a1 = a_next;
    }

    return a1;   /* a(n) */
}
