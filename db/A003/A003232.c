/*
 * A003232  Expansion of (x-1)*(x^2-4*x-1)/(1-2*x)^2.
 * https://oeis.org/A003232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (16 + 11*n) * 2^(n-3) for n >= 2, with a(0) = 1, a(1) = 7
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;
    
    seq_t numerator = (seq_t)16 + (seq_t)11 * (seq_t)n;
    if (n == 2) {
        return numerator >> 1;
    } else {
        return numerator << (n - 3);
    }
}
