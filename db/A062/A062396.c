/*
 * A062396  a(n) = 9^n + 1.
 * https://oeis.org/A062396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 9^n + 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;          /* result holds the current power of 9 */
    seq_t base   = 9;          /* base squared each loop */
    idx_t e = n;               /* exponent in unsigned form */

    while (e > 0) {
        if (e & 1U)            /* if the least significant bit of e is set */
            result = result * base;
        base = base * base;    /* square the base for next bit */
        e >>= 1;               /* shift exponent right by one bit */
    }
    return result + 1;         /* add the +1 term from the definition */
}
