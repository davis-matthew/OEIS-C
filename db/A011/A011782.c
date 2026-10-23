/*
 * A011782  Coefficients of expansion of (1-x)/(1-2*x) in powers of x.
 * https://oeis.org/A011782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy, changed
 * Formula:   a(n) = 1 if n==0 else 2^(n-1)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    seq_t value = (seq_t)1;
    /* Compute 2^(n-1) as repeated doubling. */
    for (idx_t i = 1; i < n; ++i) {
        value = value * (seq_t)2;
    }
    return value;
}
