/*
 * A048642  Partial products of A003188 (Gray code).
 * https://oeis.org/A048642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ∏_{j=1..n} (j ⊕ ⌊j/2⌋)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = (seq_t)1;        /* a(0) = 1 by definition           */
    for (idx_t j = 1; j <= n; ++j)
    {
        seq_t gray = (seq_t)j ^ ((seq_t)j >> 1);  /* j XOR (j>>1)             */
        prod = prod * gray;        /* multiply the running product  */
    }
    return prod;
}
