/*
 * A147962  a(n) = ((7+sqrt(3))^n + (7-sqrt(3))^n) / 2.
 * https://oeis.org/A147962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((7+sqrt(3))^n + (7-sqrt(3))^n)/2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=7, a(n)=14*a(n-1)-46*a(n-2) */
seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 7;
    seq_t prev2 = 1;  /* a(0) */
    seq_t prev1 = 7;  /* a(1) */
    seq_t cur;
    for (idx_t i = 2; i <= n; ++i)
    {
        cur = 14 * prev1 - 46 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
