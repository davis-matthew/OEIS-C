/*
 * A095463  a(n) = 35 written in base 11 - n.
 * https://oeis.org/A095463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = decimal value of the representation of 35 in base (11-n)
 *            interpreted as a * decimal integer. For n=0..10 this produces
 *            32,35,38,43,50,55,120,203,1022,100011, *
 *            11111111111111111111111111111111111. For n>10 the function
 *            returns 0
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 9, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* base := 11 - n */
    idx_t base = 11 - n;
    if (base <= 0) {          /* undefined for n > 10 */
        return (seq_t)0;
    }

    /* digits of 35 in base 'base', stored least-significant first */
    unsigned int digits[64];
    int cnt = 0;
    int value = 35;

    if (base == 1) {
        /* Unary representation: 35 ones */
        for (int i = 0; i < value; ++i) {
            digits[cnt++] = 1;
        }
    } else {
        do {
            digits[cnt++] = (unsigned int)(value % base);
            value /= base;
        } while (value > 0);
    }

    /* Convert digit string to decimal integer */
    seq_t result = 0;
    for (int i = cnt - 1; i >= 0; --i) {
        result = result * 10 + (seq_t)digits[i];
    }
    return result;
}
