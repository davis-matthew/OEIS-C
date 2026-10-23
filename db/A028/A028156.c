/*
 * A028156  Expansion of 1/((1-4x)(1-8x)(1-9x)(1-11x)).
 * https://oeis.org/A028156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (10*11^(n+3)-42*9^(n+3)+35*8^(n+3)-3*4^(n+3))/420
 *            Recurrence: a(n) = 32*a(n-1) - 371*a(n-2) + 1828*a(n-3) -
 *            3168*a(n-4), n ≥ 4 Initial values: a(0)=1, a(1)=32, a(2)=653,
 *            a(3)=10852
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return known initial values directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)32;
    if (n == 2) return (seq_t)653;
    if (n == 3) return (seq_t)10852;

    /* Recurrence coefficients.  They may overflow on very small
       seq_t types, but for such types the sequence values do not fit
       anyway, so the overflow does not affect correctness for the
       terms that are testable. */
    static const seq_t C1 = (seq_t)32;
    static const seq_t C2 = (seq_t)371;
    static const seq_t C3 = (seq_t)1828;
    static const seq_t C4 = (seq_t)3168;

    /* Initialize the first four terms. */
    seq_t a0 = (seq_t)1;          /* a(n-4)  */
    seq_t a1 = (seq_t)32;         /* a(n-3)  */
    seq_t a2 = (seq_t)653;        /* a(n-2)  */
    seq_t a3 = (seq_t)10852;      /* a(n-1)  */
    seq_t an = a3;                /* placeholder */

    /* Iterate from 4 up to n, applying the recurrence. */
    for (idx_t i = 4; i <= n; ++i) {
        an = (seq_t)(C1 * a3
                     - C2 * a2
                     + C3 * a1
                     - C4 * a0);
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }

    return an;
}
