/*
 * A074539  a(n) = 2^n + 5^n + 8^n.
 * https://oeis.org/A074539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n + 5^n + 8^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* exponentiation by squaring; works for any seq_t (C or C++) */
    seq_t base2 = 2, base5 = 5, base8 = 8;
    seq_t pow2 = 1, pow5 = 1, pow8 = 1;
    idx_t e = n;
    while (e) {
        if (e & 1) {
            pow2 *= base2;
            pow5 *= base5;
            pow8 *= base8;
        }
        base2 *= base2;
        base5 *= base5;
        base8 *= base8;
        e >>= 1;
    }
    return pow2 + pow5 + pow8;
}
