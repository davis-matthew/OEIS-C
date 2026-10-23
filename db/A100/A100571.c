/*
 * A100571  Cubes m^3 such that m^3 is the sum of m-1 consecutive primes plus
 *          a larger prime.
 * https://oeis.org/A100571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (m(n))³ where m(n) = n+1, except that m = 8 (512) is
 *            skipped
 * Range:     u8 n <= 5, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n + 1;
    if (m >= 8)          /* skip 8³ = 512 */
        ++m;
    seq_t mm = (seq_t)m;
    seq_t cube = mm * mm * mm;
    return cube;
}
