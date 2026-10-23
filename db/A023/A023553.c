/*
 * A023553  Convolution of integers >= 3 and Lucas numbers.
 * https://oeis.org/A023553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + 3*n + 4, with a(1) = 3, a(2) = 13
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Index offset 1 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)13;

    seq_t a_prev2 = (seq_t)3;   /* a(1) */
    seq_t a_prev1 = (seq_t)13;  /* a(2) */
    seq_t a_curr;

    for (idx_t i = 3; i <= n; ++i) {
        seq_t i_seq = (seq_t)i;
        a_curr = a_prev1 + a_prev2 + (seq_t)3 * i_seq + (seq_t)4;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_prev1;
}
