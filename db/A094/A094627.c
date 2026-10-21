/*
 * A094627  Expansion of (1+x)^2/((1-x)*(1-10*x^2)).
 * https://oeis.org/A094627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(10^floor(n/2)*(2-(-1)^n+4/9))
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is equivalent to a(n) = (10^floor(n/2)*num)/9 where
 * num = 13 if n is even, 31 if n is odd. */
seq_t a(idx_t n)
{
    /* Compute k = floor(n/2) */
    idx_t k = n / 2;

    /* Compute 10^k into value pow10 */
    seq_t pow10 = (seq_t)1;
    for (idx_t i = 0; i < k; ++i) {
        pow10 *= (seq_t)10;
    }

    /* Choose numerator based on parity of n */
    seq_t num = (n & 1) ? (seq_t)31 : (seq_t)13;

    /* Denominator is 9 for all cases */
    seq_t denom = (seq_t)9;

    /* Compute result: integer division truncates toward zero (floor for positive) */
    return (pow10 * num) / denom;
}
