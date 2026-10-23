/*
 * A179991  Nonhomogeneous three-term sequence a(n) = a(n-1) + a(n-2) + n.
 * https://oeis.org/A179991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + n with a(1) = 2, a(2) = 3 (offset = 1)
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)          /* n == 1 or n == 0 (undefined) */
        return n ? (seq_t)2 : (seq_t)0;

    /* initial values */
    seq_t prev2 = (seq_t)2;   /* a(1) */
    seq_t prev1 = (seq_t)3;   /* a(2) */

    if (n == 2)
        return prev1;

    for (idx_t i = 3; i <= n; ++i) {
        seq_t current = prev1 + prev2 + (seq_t)i;
        prev2 = prev1;
        prev1 = current;
    }

    return prev1;
}
