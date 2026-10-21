/*
 * A059304  a(n) = 2^n * (2*n)! / (n!)^2.
 * https://oeis.org/A059304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n * C(2n,n)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute binomial coefficient C(2n,n) multiplicatively
       to avoid intermediate overflow. */
    seq_t bin = 1;
    for (idx_t i = 1; i <= n; ++i) {
        bin = (bin * ((seq_t)n + i)) / i;
    }

    /* Compute 2^n by repeated left shift. */
    seq_t pow = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow <<= 1;
    }

    return bin * pow;
}
