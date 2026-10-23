/*
 * A021758  Decimal expansion of 1/754.
 * https://oeis.org/A021758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1) / 754) % 10 * gives the nth digit after
 *            the decimal point of 1/754. * Computed as a(n) = (10 * (10^n mod
 *            754)) / 754
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute r = 10^n mod 754 */
    const seq_t MOD = 754;
    seq_t r   = 1;
    seq_t b   = 10 % MOD;     /* base for exponentiation */
    idx_t e   = n;
    while (e > 0) {
        if (e & 1) {
            r = (r * b) % MOD;
        }
        b = (b * b) % MOD;
        e >>= 1;
    }
    /* digit = floor(10 * r / 754) */
    seq_t digit = (r * 10) / MOD;
    return digit;
}
