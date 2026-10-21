/*
 * A068522  In base 10 notation replace digits of n with their squared values
 *          (Version 2).
 * https://oeis.org/A068522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{i≥0} (digit_i(n))^2 * 10^i where digit_i(n) is the ith
 *            decimal digit of n (least‑significant digit has i=0)
 * Range:     u8 n <= 52, u16 n <= 8379, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t place  = 1;
    idx_t temp   = n;

    while (temp > 0) {
        unsigned d = temp % 10;          /* current decimal digit (0–9) */
        result += (seq_t)d * d * place;  /* (d^2) * 10^i */
        place *= 10;
        temp  /= 10;
    }
    return result;
}
