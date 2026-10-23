/*
 * A137139  Numbers k such that k and k^2 use only the digits 4, 5, 6 and 7.
 * https://oeis.org/A137139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, more
 * Formula:   a(1)=74, a(2)=76, a(n)=concatenate(744 repeated (n-2) times
 *            followed by 76) for n>=3
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)74;
    if (n == 2) return (seq_t)76;

    seq_t val = 0;
    for (idx_t i = 0; i < n - 2; ++i) {
        val = val * (seq_t)1000 + (seq_t)744;
    }
    val = val * (seq_t)100 + (seq_t)76;
    return val;
}
