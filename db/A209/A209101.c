/*
 * A209101  Number of 2 X n 0..2 arrays with new values 0..2 introduced in row
 *          major order and no element equal to more than one of its immediate
 *          leftward or upward or right-upward antidiagonal neighbors.
 * https://oeis.org/A209101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 8*a(n-2) for n>3 with initial values a(1)=2,
 *            a(2)=11, a(3)=76
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined offset, return 0 */
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)11;
    if (n == 3) return (seq_t)76;

    seq_t prev2 = (seq_t)11;  /* a(2) */
    seq_t prev1 = (seq_t)76;  /* a(3) */
    seq_t curr = (seq_t)0;

    for (idx_t i = 4; i <= n; ++i) {
        curr = (prev1 * (seq_t)8) - (prev2 * (seq_t)8);
        prev2 = prev1;
        prev1 = curr;
    }

    return curr;
}
