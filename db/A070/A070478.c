/*
 * A070478  a(n) = n^3 mod 16.
 * https://oeis.org/A070478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 mod 16
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n & 15U;           /* n mod 16 */
    seq_t sq = m * m;                   /* m^2 (fits in < 256) */
    seq_t cube = sq * m;                /* m^3 (fits in < 4096) */
    return cube & 15U;                  /* cube mod 16 */
}
