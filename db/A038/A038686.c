/*
 * A038686  Tenth powers ending nontrivially in a nonzero tenth power.
 * https://oeis.org/A038686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (10*floor((n-1)/2) + (n%2?9:11))^10
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    idx_t pair = (n - 1) / 2;                /* floor((n-1)/2) */
    idx_t raw_base = 10 * pair + (n & 1 ? 9 : 11);
    seq_t base = (seq_t)raw_base;

    seq_t res = 1;
    for (int i = 0; i < 10; ++i)
        res *= base;
    return res;
}
