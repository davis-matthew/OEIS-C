/*
 * A041365  Denominators of continued fraction convergents to sqrt(197).
 * https://oeis.org/A041365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 28*a(n-1) + a(n-2), a(0)=1, a(1)=28
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)28;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)28;  /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur = (seq_t)28 * prev1 + prev2; /* a(i) = 28*a(i-1) + a(i-2) */
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1; /* a(n) */
}
