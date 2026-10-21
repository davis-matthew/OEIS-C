/*
 * A062806  a(n) = Sum_{i=1..n} i*n^i.
 * https://oeis.org/A062806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} i*n^i
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    seq_t sum   = 0;
    seq_t power = (seq_t)n;   /* power = n^i */
    for (idx_t i = 1; i <= n; ++i) {
        sum += (seq_t)i * power;   /* add i * n^i */
        power *= (seq_t)n;         /* next power: n^(i+1) */
    }
    return sum;
}
