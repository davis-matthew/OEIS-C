/*
 * A190944  Multiples of 3 written in base 2.
 * https://oeis.org/A190944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = decimal value of the binary representation of 3*n (i.e.,
 *            write 3*n in base 2, then interpret that string of 0/1 digits as
 *            a decimal integer)
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 341, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* 3 * n in the arithmetic of seq_t */
    seq_t m = ((seq_t)n) * 3;
    if (m == 0) return 0;

    /* Determine how many bits are needed to represent m. */
    idx_t bits = 0;
    seq_t tmp = m;
    while (tmp) {
        ++bits;
        tmp >>= 1;
    }

    /* Build the decimal number by reading bits from most significant
       to least significant. */
    seq_t result = 0;
    for (idx_t i = bits; i > 0; --i) {
        seq_t bit = (m >> (i - 1)) & 1;
        result = result * 10 + bit;
    }
    return result;
}
