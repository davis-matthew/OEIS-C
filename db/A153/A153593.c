/*
 * A153593  a(n) = ((9 + sqrt(2))^n - (9 - sqrt(2))^n)/(2*sqrt(2)).
 * https://oeis.org/A153593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((9 + sqrt(2))^n - (9 - sqrt(2))^n)/(2*sqrt(2)) *
 *            Recurrence: a(0) = 0, a(1) = 1, * a(n) = 18*a(n-1) - 79*a(n-2)
 *            for n > 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t prev2 = 0;   /* a(0) */
    seq_t prev1 = 1;   /* a(1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i) {
        curr = 18 * prev1 - 79 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
