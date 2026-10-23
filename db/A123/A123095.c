/*
 * A123095  Sum of first n 11th powers.
 * https://oeis.org/A123095
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=1..n} k^11
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 49, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t kk = (seq_t)k;
        seq_t p2  = kk * kk;          /* k^2  */
        seq_t p3  = p2 * kk;          /* k^3  */
        seq_t p4  = p3 * kk;          /* k^4  */
        seq_t p5  = p4 * kk;          /* k^5  */
        seq_t p6  = p5 * kk;          /* k^6  */
        seq_t p7  = p6 * kk;          /* k^7  */
        seq_t p8  = p7 * kk;          /* k^8  */
        seq_t p9  = p8 * kk;          /* k^9  */
        seq_t p10 = p9 * kk;          /* k^10 */
        seq_t p11 = p10 * kk;         /* k^11 */
        sum += p11;
    }
    return sum;
}
