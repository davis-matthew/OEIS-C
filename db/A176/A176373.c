/*
 * A176373  x-values in the solution to x^2 - 67*y^2 = 1.
 * https://oeis.org/A176373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 97684*a(n-1) - a(n-2), a(1)=1, a(2)=48842
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 4, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)1;
    }
    if (n == 2) {
        return (seq_t)48842;
    }
    seq_t prev2 = (seq_t)1;      /* a(1) */
    seq_t prev1 = (seq_t)48842;  /* a(2) */
    seq_t cur;
    for (idx_t i = 3; i <= n; ++i) {
        cur = (seq_t)97684 * prev1 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
