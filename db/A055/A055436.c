/*
 * A055436  a(n) = concatenation of n^2 and n.
 * https://oeis.org/A055436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = n^2*floor(log_10(n)+1) + n
 * Range:     u8 n <= 5, u16 n <= 25, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Count decimal digits of n */
    idx_t tmp = n;
    uint64_t digits = 0;
    do {
        ++digits;
        tmp /= 10;
    } while (tmp);

    /* Compute 10^digits as seq_t */
    seq_t pow10 = (seq_t)1;
    for (uint64_t i = 0; i < digits; ++i) {
        pow10 *= (seq_t)10;
    }

    /* Compute n^2, shift left by digits and add n */
    seq_t square = (seq_t)n * (seq_t)n;
    square *= pow10;
    square += (seq_t)n;

    return square;
}
