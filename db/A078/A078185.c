/*
 * A078185  Number of n X n matrices over an alphabet of size 15.
 * https://oeis.org/A078185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15^(n^2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)15;
    seq_t exp    = (seq_t)n * (seq_t)n;
    while (exp)
    {
        if (exp & (seq_t)1)
            result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}
