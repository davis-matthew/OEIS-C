/*
 * A021177  Decimal expansion of 1/173.
 * https://oeis.org/A021177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10 × ((10^n) mod 173) / 173) That is the n-th digit
 *            after the decimal point of 1/173
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t r   = 1;              /* r = 10^n mod 173 */
    seq_t base = 10;
    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) {
            r = (r * base) % 173;
        }
        base = (base * base) % 173;
        exp >>= 1;
    }
    /* Compute the digit: floor(10 * r / 173)  (0 ≤ result ≤ 9) */
    seq_t digit = (10 * r) / 173;
    return digit;
}
