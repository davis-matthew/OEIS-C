/*
 * A052852  Expansion of e.g.f.: (x/(1-x))*exp(x/(1-x)).
 * https://oeis.org/A052852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 0, a(1) = 1 Recurrence: (n^2+2*n)*a(n) + (-4-2*n)*a(n+1)
 *            + a(n+2) = 0 for n ≥ 0 Equivalently: a(n+2) = (4+2*n)*a(n+1) -
 *            (n^2+2*n)*a(n) (0-indexed)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle base cases directly */
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* Iteratively compute the sequence from a(0) up to a(n) */
    seq_t a_prev = 0;   /* a(i)   */
    seq_t a_curr = 1;   /* a(i+1) */

    for (idx_t i = 0; i < n - 1; ++i) {          /* i runs 0 .. n-2  */
        /* coeff1 = 4 + 2*i
           coeff2 = i*i + 2*i   (both expressed as seq_t to avoid overflow) */
        seq_t coeff1 = (seq_t)4 + (seq_t)2 * i;
        seq_t coeff2 = (seq_t)i * i + (seq_t)2 * i;

        seq_t a_next = coeff1 * a_curr - coeff2 * a_prev; /* a(i+2) */

        /* Shift for next iteration */
        a_prev = a_curr;
        a_curr = a_next;
    }

    return a_curr;   /* a(n) */
}
