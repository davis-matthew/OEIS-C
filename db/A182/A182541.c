/*
 * A182541  Coefficients in g.f. for certain marked mesh patterns.
 * https://oeis.org/A182541
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)! * (2*H_{n-1} - 1)/4 where H_{m} is the m‑th
 *            harmonic number. It can also be computed by the recurrence a(3)
 *            = 1 a(n+1) = n * a(n) + (n-1)!/2 (for n ≥ 3)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)        /* values below the offset are irrelevant */
        return (seq_t)0;

    seq_t a_val = (seq_t)1;   /* a(3) */
    seq_t fact   = (seq_t)2;   /* (3-1)! = 2! */

    if (n == 3)
        return a_val;

    for (idx_t k = 3; k < n; ++k) {
        /* a(k+1) = k * a(k) + (k-1)! / 2 */
        a_val = ( (seq_t)k ) * a_val + fact / (seq_t)2;
        /* Update factorial for next step: (k+1-1)! = k! */
        fact *= (seq_t)k;
    }
    return a_val;
}
