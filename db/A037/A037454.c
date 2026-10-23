/*
 * A037454  a(n) = Sum_{i=0..m} d(i)*6^i, where Sum_{i=0..m} d(i)*3^i is the
 *          base 3 representation of n.
 * https://oeis.org/A037454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = reinterpret the base‑3 digits of n as a base‑6 number
 *            (a(n) = Σ d(i) * 6^i where n = Σ d(i) * 3^i)
 * Range:     u8 n <= 38, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res   = 0;
    seq_t power = 1;          /* 6^0 */
    while (n != 0)
    {
        idx_t digit = n % 3;          /* least‑significant base‑3 digit */
        res += (seq_t)digit * power;  /* contribution of this digit */
        power *= (seq_t)6;            /* next power of 6 */
        n /= 3;                       /* next digit */
    }
    return res;
}
