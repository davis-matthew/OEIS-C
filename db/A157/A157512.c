/*
 * A157512  Partial sums of A157502.
 * https://oeis.org/A157512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+1) + 2 * Σ_{m=1}^{⌊h(n)⌋} (n - (1 + m*(2m-1)) + 1)
 *            where the inner sum runs over the jump indices j_m = 1 + m(2m-1)
 *            ≤ n
 * Range:     u8 n <= 14, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * ((seq_t)n + 1);   /* n*(n+1) */
    seq_t S = 0;

    for (idx_t m = 1; ; ++m) {
        idx_t j = 1 + m * (2 * m - 1);        /* j_m = 1 + m(2m-1) */
        if (j > n) break;
        S += (seq_t)(n - j + 1);             /* contribution of this jump */
    }

    return base + 2 * S;
}
