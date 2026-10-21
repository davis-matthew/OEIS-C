/*
 * A079340  Absolute value of determinant of n X n matrix whose entries are
 *          the integers from 1 to n^2 spiraling outward, ending in a corner.
 * https://oeis.org/A079340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^2-3*n+3)*(2n-2)!/(2*(n-1)!), n>1 (absolute value of
 *            determinant of the n×n spirally populated matrix)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 1;                 /* a(1) = 1 */

    /* compute product P = ∏_{k=n}^{2n-2} k  (even for all n≥2) */
    seq_t P = 1;
    for (idx_t k = n; k <= 2*n - 2; ++k) {
        P *= (seq_t)k;
    }

    /* factor F = 2*n^2 - 3*n + 3, always integer (odd/even alternates) */
    seq_t F = (seq_t)(2*n*n - 3*n + 3);

    /* P is always even, so divide before the final multiplication if desired.
     * Avoids overflow in intermediate steps for some builds. */
    P /= 2;                               /* P = P / 2  */
    return P * F;                          /* final result */
}
