/*
 * A077422  Chebyshev sequence T(n,11) with Diophantine property.
 * https://oeis.org/A077422
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 22*a(n-1) - a(n-2),  a(0) = 1, a(1) = 11
 * (This is T(n,11), the Chebyshev sequence of the first kind with x=11.)
 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)11;

    seq_t prev2 = (seq_t)1;   /* a(n-2) */
    seq_t prev1 = (seq_t)11;  /* a(n-1) */
    seq_t cur;

    /* Iterate up to n */
    for (idx_t i = 2; i <= n; ++i) {
        cur = (seq_t)22 * prev1 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
