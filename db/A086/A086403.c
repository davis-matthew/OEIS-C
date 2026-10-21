/*
 * A086403  Numerators in the convergents of the continued fraction
 *          representation of (e-1)/(e+1).
 * https://oeis.org/A086403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n-2)*a(n-1) + a(n-2) with initial values a(0)=0,
 *            a(1)=1. This recurrence yields the numerators of the convergents
 *            of the continued fraction [0; 2, 6, 10, 14, 18, ...]
 *            representing (e-1)/(e+1). The sequence is indexed starting at
 *            n=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base cases directly.  The OEIS offset is 1, so for
       n=1 we return the first numerator, 1. */
    if (n == 0)   /* not used but harmless */
        return 0;
    if (n == 1)
        return 1;

    seq_t prev_prev = 0;      /* a(0) */
    seq_t prev      = 1;      /* a(1) */
    seq_t current   = 0;

    for (idx_t k = 2; k <= n; ++k) {
        /* coefficient for a(k) is 4*k - 2 (the k-th partial
           quotient of the continued fraction after the initial 0). */
        seq_t coeff   = (seq_t)(4 * k - 2);
        current = coeff * prev + prev_prev;
        prev_prev = prev;
        prev      = current;
    }
    return prev;
}
