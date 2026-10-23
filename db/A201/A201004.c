/*
 * A201004  Triangular numbers, T(m), that are five-quarters of another
 *          triangular number; T(m) such that 4*T(m) = 5*T(k) for some k.
 * https://oeis.org/A201004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 322*a(n-1) - a(n-2) + 45 (n>1), a(0) = 0, a(1) = 45
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 45;

    seq_t prev2 = 0;      /* a(0)  */
    seq_t prev1 = 45;     /* a(1)  */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i)
    {
        curr = (seq_t)322 * prev1 - prev2 + 45;
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
