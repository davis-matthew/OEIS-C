/*
 * A092985  a(n) is the product of the first n terms of an arithmetic
 *          progression with the first term 1 and common difference n.
 * https://oeis.org/A092985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ∏_{k=1..n} (1 + (k-1) * n) (a(0)=1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;          /* initial value for product, includes the empty product for n=0 */
    for (idx_t k = 1; k <= n; ++k) {
        /* Compute the k‑th factor: 1 + (k-1) * n   (all in seq_t arithmetic) */
        seq_t factor = 1 + ((seq_t)(k - 1)) * ((seq_t)n);
        res *= factor;
    }
    return res;
}
