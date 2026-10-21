/*
 * A070394  a(n) = 6^n mod 17.
 * https://oeis.org/A070394
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^n mod 17 (Euler's theorem: 6^16 ≡ 1 (mod 17))
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce exponent modulo the period 16 */
    idx_t r = n & 15;          /* same as n % 16, but faster for unsigned idx_t */

    seq_t base = 6 % 17;       /* base of the power, already < 17 */
    seq_t result = 1;          /* 6^0 = 1 */

    /* Standard binary exponentiation – safe because all intermediate
       products are below 17*17 = 289, fitting in any seq_t type. */
    while (r > 0)
    {
        if (r & 1)
            result = (result * base) % 17;
        base = (base * base) % 17;
        r >>= 1;
    }

    return result;
}
