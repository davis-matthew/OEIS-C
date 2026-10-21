/*
 * A054893  a(n) = Sum_{j > 0} floor(n/4^j).
 * https://oeis.org/A054893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{j>0} ⌊ n / 4^j ⌋ (OEIS A054893)
 * Range:     u8 n <= 771, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    if (n < 4) return 0;          /* early exit for n=0..3, request is 0 */
    idx_t div = 4;                 /* start with 4^1 */
    while (div <= n) {
        sum += (seq_t)(n / div);   /* floor division, cast to seq_t */
        /* stop before possible overflow of div *= 4 */
        if (div > n / 4) break;
        div *= 4;
    }
    return sum;
}
