/*
 * A176731  Denominators of coefficients of a series, called g, related to
 *          Airy functions.
 * https://oeis.org/A176731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 3*n*(3*n + 1)*a(n-1), a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the simple recurrence for A176731. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t result = 1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t xi = (seq_t)i;                /* cast index to seq_t */
        seq_t factor = 3 * xi;              /* 3 * i  */
        factor *= (3 * xi + 1);             /* multiply by (3*i+1) */
        result *= factor;                   /* accumulate product */
    }
    return result;
}
