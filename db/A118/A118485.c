/*
 * A118485  a(n) = 11321 - 23479*n + 16423*n^2 - 4693*n^3 + 471*n^4.
 * https://oeis.org/A118485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11321 - 23479*n + 16423*n^2 - 4693*n^3 + 471*n^4
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    long long x = (long long)n;
    long long res = 471 * x * x * x * x
                     - 4693 * x * x * x
                     + 16423 * x * x
                     - 23479 * x
                     + 11321;
    return (seq_t)res;
}
