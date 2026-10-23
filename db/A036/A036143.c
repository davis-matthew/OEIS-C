/*
 * A036143  a(n) = 3^n mod 127.
 * https://oeis.org/A036143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n mod 127
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;   /* 3^0 mod 127 = 1 */
    seq_t base   = 3;   /* base for exponentiation */
    seq_t mod    = 127; /* modulo value */

    while (n) {
        if (n & 1U)          /* if the current bit is set, multiply */
            result = (result * base) % mod;

        base = (base * base) % mod; /* square the base for next bit */
        n >>= 1U;                   /* shift to next bit */
    }
    return result;
}
