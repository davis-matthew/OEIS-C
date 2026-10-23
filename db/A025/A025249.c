/*
 * A025249  a(n) = (1/2)*s(n+3), where s = A025248.
 * https://oeis.org/A025249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((2*n+3)*a(n‑1) + (8*n-12)*a(n‑3) - n*a(n‑2)) / (n+3)
 *            Initial values: a(1)=1, a(2)=1, a(3)=3 Recurrence from OEIS:
 *            (n+3)*a(n) + (-2n-3)*a(n-1) + n*a(n-2) + 4*(-2n+3)*a(n-3) = 0
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 3) {
        if (n == 1 || n == 2)
            return (seq_t)1;
        /* n == 3 */
        return (seq_t)3;
    }

    /* a1 = a(n-3), a2 = a(n-2), a3 = a(n-1) */
    seq_t a1 = (seq_t)1; /* a(1) */
    seq_t a2 = (seq_t)1; /* a(2) */
    seq_t a3 = (seq_t)3; /* a(3) */
    seq_t val;

    for (idx_t k = 4; k <= n; ++k) {
        /* Compute numerator = (2*k+3)*a3 + (8*k-12)*a1 - k*a2 */
        seq_t term1 = ((seq_t)(2 * k + 3)) * a3;
        seq_t term2 = ((seq_t)(8 * k - 12)) * a1;
        seq_t sum12 = term1 + term2;
        seq_t term3 = ((seq_t)k) * a2;
        seq_t numerator = sum12 - term3;

        /* a(k) = numerator / (k + 3) */
        val = numerator / (k + 3);

        /* Shift for next iteration */
        a1 = a2;
        a2 = a3;
        a3 = val;
    }

    return a3;
}
