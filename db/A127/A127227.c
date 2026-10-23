/*
 * A127227  a(n)= numerator of ((n + 3)! - (n - 3)!)/(n!).
 * https://oeis.org/A127227
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = (n+3)!/(n-3)! - 1
 * Range:     u8 none, u16 n <= 6, u32 n <= 39, u64 n <= 1624, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;
    res *= ((seq_t)n - 2);
    res *= ((seq_t)n - 1);
    res *= ((seq_t)n);
    res *= ((seq_t)n + 1);
    res *= ((seq_t)n + 2);
    res *= ((seq_t)n + 3);
    return res - 1;
}
