/*
 * A076811  Number of n X n matrices over an alphabet of size 13.
 * https://oeis.org/A076811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 13^(n^2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t base = 13;
    idx_t e = n * n;
    while (e != 0) {
        if (e & 1)
            result = result * base;
        base = base * base;
        e = e >> 1;
    }
    return result;
}
