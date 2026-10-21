/*
 * A087744  Binary and decimal representation of n concatenated.
 * https://oeis.org/A087744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = decimal concatenation of binary(n) and decimal(n)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 127, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the decimal value of the binary representation of n. */
    idx_t tmp = n;
    idx_t msb = 0;
    while (tmp > 1) {
        msb++;
        tmp >>= 1;
    }

    seq_t binVal = 0;
    for (idx_t i = msb; ; i--) {
        binVal = binVal * 10 + (seq_t)((n >> i) & 1);
        if (i == 0) break;
    }

    /* Determine the number of decimal digits of n. */
    idx_t t = n;
    idx_t len = 0;
    do {
        len++;
        t /= 10;
    } while (t);

    /* Compute 10^len. */
    seq_t pow10 = 1;
    for (idx_t i = 0; i < len; i++) {
        pow10 *= 10;
    }

    return binVal * pow10 + (seq_t)n;
}
