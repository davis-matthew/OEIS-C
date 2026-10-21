/*
 * A211264  Number of integer pairs (x,y) such that 0 < x < y <= n and x*y <=
 *          n.
 * https://oeis.org/A211264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n} max(0, ⌊n/i⌋ − i) Count the number of pairs
 *            (x,y) with 0 < x < y ≤ n and x*y ≤ n
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t q = ((seq_t)n) / ((seq_t)i);   /* floor(n / i) */
        if (q <= i)                         /* no further pairs possible */
            break;
        result += q - ((seq_t)i);
    }
    return result;
}
