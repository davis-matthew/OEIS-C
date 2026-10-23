/*
 * A104356  Smallest m such that A104350(m) has exactly n trailing zeros in
 *          decimal representation.
 * https://oeis.org/A104356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1, if n = 0; a(n) = 5*n for 0 < n < 5; a(n) = 2^n
 *            otherwise
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    if (n < 5)
        return (seq_t)5 * (seq_t)n;

    seq_t res = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        res = res * 2;
    return res;
}
