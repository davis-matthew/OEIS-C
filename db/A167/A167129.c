/*
 * A167129  n^7 mod 21.
 * https://oeis.org/A167129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^7 mod 21
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 21 first; the remainder is in [0,20] */
    idx_t r = n % 21;
    seq_t res = 1;
    seq_t base = (seq_t)r;

    /* Compute base^7 modulo 21 */
    for (idx_t i = 0; i < 7; ++i) {
        res = (res * base) % 21;
    }
    return res;
}
