/*
 * A198972  a(n) = (7*10^n - 1)/3.
 * https://oeis.org/A198972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*10^n - 1)/3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 2; a(n) = 10*a(n-1) + 3 for n ≥ 1 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 2;

    seq_t val = 2;

    for (idx_t i = 1; i <= n; ++i)
    {
        val *= 10;
        val += 3;
    }

    return val;
}
