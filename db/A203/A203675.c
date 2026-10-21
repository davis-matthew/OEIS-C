/*
 * A203675  Vandermonde sequence using x^2 - xy + y^2 applied to
 *          (1,4,9,...,n^2).
 * https://oeis.org/A203675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{1≤i<j≤n} (i^4 - i^2 j^2 + j^4)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of the Vandermonde sequence
   using x^2 - xy + y^2 applied to (1,4,9,...,n^2). */
seq_t a(idx_t n)
{
    if (n <= 1) {
        return (seq_t)1;
    }

    seq_t prod = (seq_t)1;
    for (idx_t i = 1; i < n; ++i) {
        seq_t i2 = (seq_t)i * i;      /* i^2  */
        seq_t i4 = i2 * i2;           /* i^4  */
        for (idx_t j = i + 1; j <= n; ++j) {
            seq_t j2 = (seq_t)j * j;  /* j^2  */
            seq_t j4 = j2 * j2;       /* j^4  */
            seq_t term = i4 - i2 * j2 + j4;  /* i^4 - i^2*j^2 + j^4 */
            prod *= term;
        }
    }
    return prod;
}
