/*
 * A005609  Number of Boolean functions realized by cascades of n gates.
 * https://oeis.org/A005609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 28*a(n-1) - 48, a(1) = 64 (equivalently a(n) = 4*(4 +
 *            5*28^n)/9)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t v = 64;
    idx_t i;
    for (i = 2; i <= n; i++)
        v = v * 28 - 48;
    return v;
}
