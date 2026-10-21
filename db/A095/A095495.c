/*
 * A095495  a(n) = 51 written in base 12 - n.
 * https://oeis.org/A095495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 51 written in base (12 - n) (digits interpreted as a
 *            decimal integer)
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the base 12 - n safely.
       If n >= 12 the base is <= 0, and the sequence is undefined – return 0. */
    if (n >= 12u) {
        return (seq_t)0;
    }
    int base = 12 - (int)n;          /* base is in [1,11] */

    /* Base 1: unary representation – 51 ones concatenated. */
    if (base == 1) {
        seq_t res = 0;
        for (idx_t i = 0; i < 51; ++i) {
            res = res * 10 + 1;
        }
        return res;
    }

    /* For bases 2 through 11: convert 51 to the desired base. */
    seq_t value = (seq_t)51;
    int digits[64];
    int len = 0;

    while (value != (seq_t)0) {
        seq_t q = value / (seq_t)base;
        int r = (int)(value - q * (seq_t)base);   /* remainder in 0..base-1 */
        digits[len++] = r;
        value = q;
    }

    /* Build the decimal integer from the reversed digit array. */
    seq_t result = 0;
    for (int i = len - 1; i >= 0; --i) {
        result = result * 10 + (seq_t)digits[i];
    }
    return result;
}
