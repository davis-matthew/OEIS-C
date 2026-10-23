/*
 * A179135  a(n) = (3-sqrt(5))*((3+sqrt(5))/10)^(-n)/2 +
 *          (3+sqrt(5))*((3-sqrt(5))/10)^(-n)/2.
 * https://oeis.org/A179135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 15*a(n-1) - 25*a(n-2); a(0) = 3; a(1) = 35
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)35;
    seq_t prev2 = (seq_t)3;    /* a(0) */
    seq_t prev1 = (seq_t)35;   /* a(1) */
    seq_t cur;
    for (idx_t i = 2; i <= n; ++i) {
        cur = (seq_t)15 * prev1 - (seq_t)25 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
