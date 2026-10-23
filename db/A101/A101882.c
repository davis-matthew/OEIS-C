/*
 * A101882  Write three numbers, skip one, write three, skip two, write three,
 *          skip three... and so on.
 * https://oeis.org/A101882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n + floor((n-1)/3) * (floor((n-1)/3) + 1) / 2
 * Range:     u8 n <= 60, u16 n <= 1077, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* below offset; unspecified */
    idx_t k = (n - 1) / 3;       /* integer floor((n-1)/3) */
    seq_t kk = (seq_t)k;
    seq_t add = kk * (kk + 1) / 2;   /* k*(k+1)/2 */
    return (seq_t)n + add;
}
