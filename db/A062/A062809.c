/*
 * A062809  a(n) = Sum_{i=1..n} (n - i)^(1 + i).
 * https://oeis.org/A062809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} (n - i)^(1 + i)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 27, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;                               /* accumulated result */
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t base = (seq_t)(n - i);             /* (n - i) */
        idx_t  exp  = i + 1;                     /* (1 + i) */

        /* compute base^exp by successive multiplication */
        seq_t term = 1;
        for (idx_t e = 0; e < exp; ++e)
            term *= base;

        sum += term;
    }
    return sum;
}
