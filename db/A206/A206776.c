/*
 * A206776  a(n) = 3*a(n-1) + 2*a(n-2) for n>1, a(0)=2, a(1)=3.
 * https://oeis.org/A206776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 2*a(n-2); a(0)=2, a(1)=3
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)2;
    }
    if (n == 1) {
        return (seq_t)3;
    }
    seq_t a_prev2 = (seq_t)2;  /* a(0) */
    seq_t a_prev1 = (seq_t)3;  /* a(1) */
    seq_t a_curr;

    for (idx_t i = 2; i <= n; ++i) {
        a_curr = (seq_t)(3 * a_prev1 + 2 * a_prev2);
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_curr;
}
