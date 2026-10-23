/*
 * A010795  a(n) = n!*(n+6)! / 6!.
 * https://oeis.org/A010795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!*(n+6)! / 6!
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by the recurrence a(0)=1, a(n)=a(n-1)*n*(n+6) */
seq_t a(idx_t n)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= n; ++i)
    {
        res *= (seq_t)i;
        res *= (seq_t)(i + 6);
    }
    return res;
}
