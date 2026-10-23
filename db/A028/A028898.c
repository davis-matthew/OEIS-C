/*
 * A028898  Map n = Sum c_i 10^i to a(n) = Sum c_i 3^i.
 * https://oeis.org/A028898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = Σ_i c_i 3^i (c_i are the decimal digits of n)
 * Range:     u8 n <= 6786, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t pow3   = 1;          /* 3^0 */

    /* Handle n = 0 explicitly – the loop below would skip it. */
    if (n == 0)
        return 0;

    while (n > 0) {
        unsigned digit = (unsigned)(n % 10);   /* least‑significant decimal digit */
        result += (seq_t)digit * pow3;
        pow3 *= 3;
        n /= 10;
    }

    return result;
}
