/*
 * A094823  If n = c0 + c1*10 + c2*10^2 + ...cn*10^n then a(n) = c0 + c1*13 +
 *          c2*13^2 + ...cn*13^k.
 * https://oeis.org/A094823
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = interpret decimal digits of n as base‑13 digits. That is,
 *            if n = c0 + c1*10 + c2*10^2 + …, then a(n) = c0 + c1*13 +
 *            c2*13^2 + …
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;      /* accumulated value                       */
    seq_t power  = 1;      /* current 13^i factor                      */
    while (n > 0)
    {
        /* n is idx_t (unsigned 64‑bit); n%10 is a digit 0..9   */
        idx_t digit = n % 10;
        result += (seq_t)digit * power;
        power  *= 13;
        n /= 10;
    }
    return result;
}
