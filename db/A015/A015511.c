/*
 * A015511  a(1) = 1, a(n) = Sum_{k=1..n-1} ((9^k - 1)/8)*a(k).
 * https://oeis.org/A015511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(2) = 1, a(n) = ((9^(n-1) + 7)/8) * a(n-1) for n ≥ 3
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2)
        return (seq_t)1;

    seq_t prev = (seq_t)1;          /* a(2) */
    seq_t pow9 = (seq_t)9;          /* 9^1, will be raised to 9^(i-1) */

    for (idx_t i = 3; i <= n; ++i) {
        pow9 = pow9 * (seq_t)9;                /* now 9^(i-1) */
        seq_t mult = (pow9 + (seq_t)7) / (seq_t)8;
        prev = prev * mult;
    }

    return prev;                       /* a(n) */
}
