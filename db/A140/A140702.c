/*
 * A140702  Main diagonal of array A(k,n) = product of first n centered
 *          n-gonal numbers.
 * https://oeis.org/A140702
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = product_{m=1}^n (n*m*(m-1)/2 + 1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = 1;
    for (idx_t m = 1; m <= n; ++m)
    {
        seq_t term = ((seq_t)n * (seq_t)m * ((seq_t)m - 1)) / 2 + 1;
        prod *= term;
    }
    return prod;
}
