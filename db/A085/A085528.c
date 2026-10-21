/*
 * A085528  a(n) = (2*n+1)^(n+1).
 * https://oeis.org/A085528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n+1)^(n+1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn    = (seq_t)n;   /* n promoted to seq_t so no overflow */
    seq_t base  = sn * 2 + 1; /* 2*n+1 */
    seq_t result = 1;

    /* exponent = n + 1 */
    for (idx_t i = 0; i < n + 1; ++i)
    {
        result *= base;
    }
    return result;
}
