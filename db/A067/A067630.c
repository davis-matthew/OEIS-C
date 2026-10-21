/*
 * A067630  a(n) = (4*n)! / 4^n.
 * https://oeis.org/A067630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*n)! / 4^n Recurrence: a(0) = 1; a(n) = (64*n^4 -
 *            96*n^3 + 44*n^2 - 6*n) * a(n-1) (n ≥ 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    seq_t result = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t kk = (seq_t)k;
        seq_t coeff =
            ((seq_t)64 * kk * kk * kk * kk)
            - ((seq_t)96 * kk * kk * kk)
            + ((seq_t)44 * kk * kk)
            - ((seq_t)6  * kk);
        result *= coeff;
    }
    return result;
}
