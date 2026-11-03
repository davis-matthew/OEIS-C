/*
 * A021471  Decimal expansion of 1/467.
 * https://oeis.org/A021471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor((10^(n) mod 467 * 10) / 467)
 * Range:     u8 n <= 7, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Validated: partial, pass, 99 terms through n = 98 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t mod_pow10_mod467(idx_t exp) {
    /* Compute 10^exp mod 467, result fits in a few bits. */
    idx_t result = 1;
    idx_t base   = 10 % 467;   /* =10 */
    while (exp) {
        if (exp & 1)
            result = (result * base) % 467;
        base = (base * base) % 467;
        exp >>= 1;
    }
    return (seq_t)result;
}
/* The sequence is the digits after the decimal point of 1/467. */

seq_t a(idx_t n) {
    /* Get r = 10^n mod 467 */
    seq_t r = mod_pow10_mod467(n);
    /* Digit: floor((r * 10) / 467) which is 0..9 */
    return (r * 10) / 467;
}
