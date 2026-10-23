/*
 * A135640  Powers of 839: a(n) = 839^n.
 * https://oeis.org/A135640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 839^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t base   = 839;
    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1U)
            result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}
