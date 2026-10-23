/*
 * A100340  Numerators of the convergents in the continued fraction expansion
 *          for the constant given by A100338, where the partial quotients
 *          equal A006519 (greatest power of 2 dividing n).
 * https://oeis.org/A100340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  cofr, nonn
 * Formula:   a(n) = a(n-1)*A006519(n) + a(n-2), a(1)=1, a(2)=3; where
 *            A006519(n) is the greatest power of 2 dividing n
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 27, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;

    seq_t prev2 = (seq_t)1;
    seq_t prev1 = (seq_t)3;
    seq_t curr = 0;

    for (idx_t i = 3; i <= n; ++i) {
        idx_t t = i & -(idx_t)i;           /* greatest power of 2 dividing i */
        curr = prev1 * (seq_t)t + prev2;
        prev2 = prev1;
        prev1 = curr;
    }

    return curr;
}
