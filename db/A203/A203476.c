/*
 * A203476  a(n) = v(n+1)/v(n), where v = A203475.
 * https://oeis.org/A203476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{j=1..n} ((n+1)^2 + j^2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;  /* offset starts at 1; unused but harmless */
    seq_t res   = 1;
    seq_t np1   = (seq_t)(n + 1);
    seq_t np1_sq = np1 * np1;

    for (idx_t j = 1; j <= n; ++j) {
        seq_t j_sq = (seq_t)j * (seq_t)j;
        res *= (np1_sq + j_sq);
    }
    return res;
}
