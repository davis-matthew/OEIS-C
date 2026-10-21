/*
 * A081090  a(n)^2 + 1 = A081089(n), where A081089(n) =
 *          A081088(n+1)/A081088(n); involves the partial quotients of a
 *          series of continued fractions that sum to unity.
 * https://oeis.org/A081090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2)*(a(n-1)^2 + 1) for n>3, with a(1)=0, a(2)=2,
 *            a(3)=5
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)5;

    seq_t a_prev2 = (seq_t)2; /* a(2) */
    seq_t a_prev1 = (seq_t)5; /* a(3) */
    seq_t a_curr;

    for (idx_t i = 4; i <= n; ++i) {
        seq_t temp = a_prev1 * a_prev1 + (seq_t)1; /* (a(i-1))^2 + 1 */
        a_curr = a_prev2 * temp;                   /* a(i) = a(i-2)*temp */
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_prev1;
}
