/*
 * A152627  Decimal expansion of 3/4.
 * https://oeis.org/A152627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor((3*10^(n+1))/4) mod 10 (decimal digits of 3/4)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* compute 10^(n+1) mod 40 */
    idx_t e = n + (idx_t)1;               /* exponent */
    seq_t base = (seq_t)10 % (seq_t)40;   /* 10 mod 40 */
    seq_t result = (seq_t)1;
    seq_t mod = (seq_t)40;

    while (e) {
        if (e & (idx_t)1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        e >>= (idx_t)1;
    }

    /* compute floor( (3 * result) / 4 )  where result = 10^(n+1) mod 40 */
    seq_t r = ( (seq_t)3 * result ) % mod;  /* r = 3*10^(n+1) mod 40 */
    seq_t digit = r / (seq_t)4;              /* floor(r/4) gives the digit 0-9 */

    return digit;
}
