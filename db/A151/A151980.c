/*
 * A151980  Numbers k such that k^2 - k is divisible by 20.
 * https://oeis.org/A151980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*floor(n/4) + 4*n - 3*floor(n/2) - 4
 * Range:     u8 n <= 51, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t1  = nn / (seq_t)4;        // floor(n/4)
    seq_t t2  = nn / (seq_t)2;        // floor(n/2)
    seq_t result = (seq_t)10 * t1 + (seq_t)4 * nn - (seq_t)3 * t2 - (seq_t)4;
    return result;
}
