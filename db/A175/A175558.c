/*
 * A175558  a(n) = 167^n.
 * https://oeis.org/A175558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 167^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t base   = (seq_t)167;
    idx_t e = n;
    while (e) {
        if (e & 1) {
            result *= base;
        }
        base *= base;
        e >>= 1;
    }
    return result;
}
