/*
 * A037496  Base-3 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,0,2.
 * https://oeis.org/A037496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 3*a(n-1) + a(n-3) - 3*a(n-4) ; Initial values: a(1)=1,
 *            a(2)=3, a(3)=11, a(4)=34
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A037496 – the base‑3 integer formed by the first n terms
   of the periodic sequence 1,0,2,1,0,2,... */
seq_t a(idx_t n)
{
    /* Offset is 1, so n must be >= 1. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)11;
    if (n == 4) return (seq_t)34;

    /* Recurrence uses the last four values only. */
    seq_t a1 = (seq_t)1;   /* a(n-4) */
    seq_t a2 = (seq_t)3;   /* a(n-3) */
    seq_t a3 = (seq_t)11;  /* a(n-2) */
    seq_t a4 = (seq_t)34;  /* a(n-1) */
    seq_t an = a4;         /* placeholder for a(n) */

    for (idx_t i = 5; i <= n; ++i) {
        an = 3 * a4 + a2 - 3 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }
    return an;
}
