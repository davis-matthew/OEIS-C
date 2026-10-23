/*
 * A033459  OR-convolution of squares A000290 with themselves.
 * https://oeis.org/A033459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0}^n (((k+1)^2) | ((n-k+1)^2))
 * Range:     u8 n <= 6, u16 n <= 34, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        /* (k+1)^2  and  (n-k+1)^2  are computed as seq_t to avoid overflow */
        seq_t kk = (seq_t)(k + 1);
        seq_t x  = kk * kk;

        seq_t mm = (seq_t)(n - k + 1);
        seq_t y  = mm * mm;

        sum += (x | y);
    }
    return sum;
}
