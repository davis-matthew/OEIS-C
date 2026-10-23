/*
 * A021895  Decimal expansion of 1/891.
 * https://oeis.org/A021895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor( (10^n mod 891) * 10 / 891 )
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth decimal digit of 1/891, zero‑based. */

static idx_t powmod10(idx_t exp) {
    idx_t result = 1;
    idx_t base = 10;
    while (exp > 0) {
        if (exp & 1) {
            result = (result * base) % 891;
        }
        base = (base * base) % 891;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    idx_t r = powmod10(n);          /* r = 10^n mod 891 */
    idx_t digit = (r * 10) / 891;   /* digit extraction */
    return (seq_t)digit;
}
