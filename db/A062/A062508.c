/*
 * A062508  a(n) = 3^(2*n) + 7.
 * https://oeis.org/A062508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3^(2*n) + 7. Equivalent linear recurrence: a(0) = 8, a(1)
 *            = 16, a(n) = 10*a(n-1) - 9*a(n-2) (n ≥ 2)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)8;
    if (n == 1) return (seq_t)16;

    seq_t prev2 = (seq_t)8;   /* a(0) */
    seq_t prev1 = (seq_t)16;  /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = (seq_t)10 * prev1 - (seq_t)9 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
