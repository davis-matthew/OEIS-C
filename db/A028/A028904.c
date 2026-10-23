/*
 * A028904  Map n = Sum c_i 10^i to a(n) = Sum c_i 9^i.
 * https://oeis.org/A028904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   * a(n) = Σ_{i=0}^{∞} (digit_i of n in base 10) * 9^i * where
 *            digit_0 is the least‑significant decimal digit. * The function
 *            computes this value for any non‑negative n. It works * for
 *            64‑bit indices and for arbitrary‑precision seq_t values
 * Range:     u8 n <= 313, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t pow = 1;
    idx_t t = n;
    while (t > 0)
    {
        unsigned digit = t % 10;      /* current decimal digit */
        result += ((seq_t)digit) * pow;
        pow *= 9;                     /* next power of 9 */
        t /= 10;
    }
    return result;
}
