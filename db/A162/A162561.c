/*
 * A162561  a(n) = ((4+sqrt(3))*(5+sqrt(3))^nv+v(4-sqrt(3))*(5-sqrt(3))^n)/2.
 * https://oeis.org/A162561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 22*a(n-2); a(0)=4, a(1)=23
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)4;
    if (n == 1) return (seq_t)23;

    seq_t prev2 = (seq_t)4; /* a(0) */
    seq_t prev1 = (seq_t)23; /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i)
    {
        cur = prev1 * (seq_t)10 - prev2 * (seq_t)22;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
