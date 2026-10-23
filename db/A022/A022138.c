/*
 * A022138  Fibonacci sequence beginning 5, 13.
 * https://oeis.org/A022138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2); a(0)=5, a(1)=13
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base cases explicitly */
    if (n == 0) return (seq_t)5U;
    if (n == 1) return (seq_t)13U;

    /* Iterate the recurrence from 2 up to n inclusive */
    seq_t prev2 = (seq_t)5U;   /* a(0) */
    seq_t prev1 = (seq_t)13U;  /* a(1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; i++) {
        curr = prev1 + prev2;  /* a(i) = a(i-1) + a(i-2) */
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
