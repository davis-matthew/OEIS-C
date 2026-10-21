/*
 * A059141  A hierarchical sequence (W2{3}*c - see A059126).
 * https://oeis.org/A059141
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 11 if n is even a(n) = 11 + 7 * (trailing_zeros((n+1)/2)
 *            + 1) if n is odd where trailing_zeros(x) counts how many times x
 *            is divisible by 2
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Even indices are always 11 */
    if ((n & 1ULL) == 0) {
        return (seq_t)11;
    }

    /* For odd n: compute m = (n+1)/2 without overflow */
    seq_t m = ((seq_t)(n >> 1)) + (seq_t)1;   /* (n+1)/2 */

    /* Count trailing zeros of m */
    unsigned tz = 0;
    while ((m & (seq_t)1) == 0) {    /* m is even */
        m >>= 1;
        ++tz;
    }

    /* a(n) = 11 + 7 * (tz + 1) */
    return (seq_t)11 + (seq_t)7 * (tz + 1);
}
