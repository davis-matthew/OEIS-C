/*
 * A021423  Decimal expansion of 1/419.
 * https://oeis.org/A021423
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = digit at position n in the decimal expansion of 1/419 * =
 *            floor((10^(n+1) / 419)) mod 10 * = ((10^n mod 419) * 10) / 419
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute rem = 10^n mod 419 using binary exponentiation */
    idx_t rem  = 1;
    idx_t base = 10 % 419;          /* 10 */
    idx_t exp  = n;
    while (exp) {
        if (exp & 1U)
            rem = (rem * base) % 419;
        base = (base * base) % 419;
        exp >>= 1;
    }
    /* digit = (rem * 10) / 419  (integer division gives 0..9) */
    seq_t digit = (seq_t)((rem * 10) / 419);
    return digit;
}
