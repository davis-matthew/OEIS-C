/*
 * A035023  One ninth of 9-factorial numbers.
 * https://oeis.org/A035023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 9^(n-1) * n!; Recurrence: a(n) = 9*n*a(n-1), a(1)=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* sequence offset starts at 1 */
    seq_t result = 1;                     /* a(1) = 1 */
    for (idx_t i = 2; i <= n; ++i) {
        result *= (seq_t)(9 * i);          /* a(i) = 9*i*a(i-1) */
    }
    return result;
}
