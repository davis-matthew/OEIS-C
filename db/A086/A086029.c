/*
 * A086029  a(n) = Sum_{i=1..n} C(i+6,7)^2.
 * https://oeis.org/A086029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{i=1..n} C(i+6,7)^2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 13, u64 n <= 68, u128 n <= 1379,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    if (n == 0) return sum;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t bin = 1;
        /* Compute C(i+6,7) multiplicatively.  k=7 is small and fixed. */
        for (idx_t j = 1; j <= 7; ++j) {
            bin = bin * (i + 7 - j) / j;
        }
        sum += bin * bin;
    }
    return sum;
}
