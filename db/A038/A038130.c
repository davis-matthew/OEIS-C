/*
 * A038130  Beatty sequence for 2*Pi.
 * https://oeis.org/A038130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(2*Pi*n)
 * Range:     u8 n <= 40, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use the rational approximation 2*Pi ≈ 6283185307179586490 / 10^18.
       The error is < 1/10^18, so for any n <= 5*10^17 the integer division
       gives the exact floor value. */
    static const unsigned long long P = 6283185307179586490ULL;
    static const unsigned long long Q = 1000000000000000000ULL;
    return (seq_t)((unsigned __int128)P * n / Q);
}
