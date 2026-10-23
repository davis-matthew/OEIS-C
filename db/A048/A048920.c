/*
 * A048920  Indices of heptagonal numbers (A000566) which are also 9-gonal.
 * https://oeis.org/A048920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(2) = 104, a(n) = 142*a(n-1) - a(n-2) - 42 for n >= 3
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)104;

    seq_t prev2 = (seq_t)1;      /* a(1) */
    seq_t prev1 = (seq_t)104;    /* a(2) */
    seq_t cur = 0;
    const seq_t coef142 = (seq_t)142;
    const seq_t const42  = (seq_t)42;

    for (idx_t i = 3; i <= n; ++i) {
        cur = coef142 * prev1 - prev2 - const42;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
