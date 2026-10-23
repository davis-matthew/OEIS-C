/*
 * A140701  Partial products of A005448.
 * https://oeis.org/A140701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(n) = a(n-1) * (1 + 3*(n-1)*n/2) for n >= 1
 *            equivalently a(n) = ∏_{k=1}^{n} (1 + 3*(k-1)*k/2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t prod = 1;
    for (idx_t k = 1; k <= n; ++k) {
        /* Compute A005448(k) = 1 + 3*(k-1)*k/2 safely within 64 bits */
        seq_t term = (seq_t)(3ULL * (k - 1) * k / 2 + 1);
        prod = prod * term;
    }
    return prod;
}
