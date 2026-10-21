/*
 * A204095  Numbers whose base 10 digits are a subset of {0, 8}.
 * https://oeis.org/A204095
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 8 * sum_{i} bit_i(n) * 10^i (i.e. a(n) = 8 * A007088(n),
 *            numbers with decimal digits only 0 or 8)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 511, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow10 = 1;       /* 10^i in seq_t */
    seq_t res   = 0;       /* sum of selected powers of 10 */

    /* Interpret bits of n as decimal digits 0/1. */
    while (n != 0) {
        if (n & 1U)
            res += pow10;
        n >>= 1;
        pow10 *= (seq_t)10;
    }

    /* Scale by 8 to map 0/1 digits to 0/8 digits. */
    return res * (seq_t)8;
}
