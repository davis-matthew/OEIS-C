/*
 * A008894  3x - 1 sequence starting at 36.
 * https://oeis.org/A008894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 36; a(n) = a(n-1)/2 if a(n-1) even else 3*a(n-1)-1
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 36;
    for (idx_t i = 1; i <= n; ++i) {
        if (val % 2 == 0)
            val = val / 2;
        else
            val = val * 3 - 1;
    }
    return val;
}
