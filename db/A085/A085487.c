/*
 * A085487  a(n) = p^n + q^n, p = (1 + sqrt(21))/2, q = (1 - sqrt(21))/2.
 * https://oeis.org/A085487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 5*a(n-2), a(1) = 1, a(2) = 11
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)11;

    seq_t prev2 = (seq_t)1;  /* a(1) */
    seq_t prev1 = (seq_t)11; /* a(2) */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t curr = prev1 + (seq_t)5 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
