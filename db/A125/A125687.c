/*
 * A125687  The base 6 numbers 4 44 444 4444 44444 ... converted to base 10.
 * https://oeis.org/A125687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*(6^n - 1)/5
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 4, a(2) = 28, a(n) = 7*a(n-1) - 6*a(n-2) for n > 2 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1, behaviour for n<1 unused */
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)28;
    seq_t prev2 = (seq_t)4;          /* a(1) */
    seq_t prev1 = (seq_t)28;         /* a(2) */
    seq_t curr = 0;
    for (idx_t i = 3; i <= n; ++i) {
        curr = (seq_t)7 * prev1 - (seq_t)6 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
