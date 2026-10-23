/*
 * A041613  Denominators of continued fraction convergents to sqrt(325).
 * https://oeis.org/A041613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 36*a(n-1) + a(n-2), a(0) = 1, a(1) = 36
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the two base values */
    if (n == (idx_t)0)
        return (seq_t)1;
    if (n == (idx_t)1)
        return (seq_t)36;

    /* iteratively compute the sequence up to n */
    seq_t prev2 = (seq_t)1;   /* a(n-2) */
    seq_t prev1 = (seq_t)36;  /* a(n-1) */
    seq_t cur;

    for (idx_t i = (idx_t)2; i <= n; ++i) {
        cur = (seq_t)36 * prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
