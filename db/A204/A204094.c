/*
 * A204094  Numbers whose set of base 10 digits is {0,7}.
 * https://oeis.org/A204094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 7 * A007088(n)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 511, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Count the number of bits in n (i.e., position of the most significant bit). */
    idx_t bits = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        bits++;
        tmp >>= 1;
    }

    seq_t result = 0;
    /* Process each bit from most significant to least significant. */
    while (bits--) {
        seq_t digit = ((n >> bits) & 1ULL) ? (seq_t)7 : (seq_t)0;
        result = result * (seq_t)10 + digit;
    }
    return result;
}
