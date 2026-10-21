/*
 * A239035  Product of 8 consecutive integers. a(n) = RisingFactorial(n, 8).
 * https://oeis.org/A239035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*(n+3)*(n+4)*(n+5)*(n+6)*(n+7)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 12, u64 n <= 252, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t v = (seq_t)n;
    v *= (seq_t)(n + 1);
    v *= (seq_t)(n + 2);
    v *= (seq_t)(n + 3);
    v *= (seq_t)(n + 4);
    v *= (seq_t)(n + 5);
    v *= (seq_t)(n + 6);
    v *= (seq_t)(n + 7);
    return v;
}
