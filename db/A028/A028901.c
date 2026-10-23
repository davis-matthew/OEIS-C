/*
 * A028901  Map n = Sum c_i*10^i to a(n) = Sum c_i*6^i.
 * https://oeis.org/A028901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Σ_{i≥0} d_i * 6^i where d_i are the decimal digits of n
 *            (n = Σ d_i * 10^i). This reinterprets n’s base‑10 digits as a
 *            base‑6 number with the same digits
 * Range:     u8 n <= 663, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t power  = 1;          /* 6^i in seq_t */
    idx_t temp   = n;          /* work on a copy of n */

    while (temp > 0)
    {
        seq_t digit = (seq_t)(temp % 10);  /* current decimal digit */
        result += digit * power;           /* accumulate contribution */
        power *= 6;                        /* next power of 6 */
        temp /= 10;                        /* move to next digit */
    }
    return result;
}
