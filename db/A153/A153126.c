/*
 * A153126  Sums of rows of the triangle in A153125.
 * https://oeis.org/A153126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(5*n+7)/2 + 1 - (n mod 2)
 * Range:     u8 n <= 6, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = (seq_t)n * ((((seq_t)5) * n) + (seq_t)7);
    seq_t res  = prod / (seq_t)2 + (seq_t)1;
    if (n & (idx_t)1) {
        res -= (seq_t)1;
    }
    return res;
}
