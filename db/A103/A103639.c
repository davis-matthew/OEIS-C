/*
 * A103639  a(n) = Product_{i=1..2*n} (2*i+1).
 * https://oeis.org/A103639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i=1..2*n} (2*i+1) (odd double factorial)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t mul1 = ((seq_t)k * 4) - 1; /* 4k-1 */
        seq_t mul2 = ((seq_t)k * 4) + 1; /* 4k+1 */
        res *= mul1 * mul2;
    }
    return res;
}
