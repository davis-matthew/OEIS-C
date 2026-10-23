/*
 * A009975  Powers of 31: a(n) = 31^n.
 * https://oeis.org/A009975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 31^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t base   = (seq_t)31;
    idx_t e = n;
    while (e != 0) {
        if (e & 1)
            result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}
