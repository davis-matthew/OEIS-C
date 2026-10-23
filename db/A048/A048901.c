/*
 * A048901  Indices of hexagonal numbers which are also heptagonal.
 * https://oeis.org/A048901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 322*a(n-1) - a(n-2) - 80
   with initial values a(1) = 1, a(2) = 247 */

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    if (n == 2) return (seq_t)247;

    seq_t prev2 = (seq_t)1;   /* a(n-2) when n == 3 */
    seq_t prev1 = (seq_t)247; /* a(n-1) when n == 3 */
    seq_t curr  = 0;

    for (idx_t i = 3; i <= n; ++i)
    {
        curr = (seq_t)322 * prev1 - prev2 - (seq_t)80;
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
