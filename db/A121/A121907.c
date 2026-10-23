/*
 * A121907  Expansion of g.f.: (1 + x + x^2)/(1 - 2*x - 2*x^2).
 * https://oeis.org/A121907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=3, a(2)=9, a(n) = 2*a(n-1) + 2*a(n-2) for n>=3
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)9;

    seq_t a_prev2 = (seq_t)3; /* a(1) */
    seq_t a_prev1 = (seq_t)9; /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t curr = (seq_t)2 * a_prev1 + (seq_t)2 * a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = curr;
    }
    return a_prev1;
}
