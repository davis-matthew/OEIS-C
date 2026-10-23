/*
 * A116689  Partial sums of dodecahedral numbers (A006566).
 * https://oeis.org/A116689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=0..n} i*(3*i-1)*(3*i-2)/2
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 248, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 0; i <= n; ++i) {
        seq_t ii = (seq_t)i;
        seq_t term = ii * ((((seq_t)3) * ii) - 1) * ((((seq_t)3) * ii) - 2);
        term /= 2;
        sum += term;
    }
    return sum;
}
