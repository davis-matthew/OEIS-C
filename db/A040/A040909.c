/*
 * A040909  Continued fraction for sqrt(940).
 * https://oeis.org/A040909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, mult, easy
 * Formula:   a(0)=30; for n>0, a(n) = cf[(n-1) mod 8] where cf =
 *            {1,1,1,14,1,1,1,60}
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th term of A040909 (continued fraction for sqrt(940)) */
seq_t a(idx_t n)
{
    static const seq_t period[] = {1,1,1,14,1,1,1,60};
    static const idx_t period_len = sizeof(period) / sizeof(period[0]);

    if (n == 0) {
        return (seq_t)30;
    }
    return period[(n - 1) % period_len];
}
