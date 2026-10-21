/*
 * A051801  Product of the nonzero digits of n.
 * https://oeis.org/A051801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base, nice
 * Formula:   a(n) = product of the non‑zero decimal digits of n with the
 *            convention a(0) = 1 (empty product)
 * Range:     u8 n <= 487, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the special case n == 0 */
    if (n == 0) return (seq_t)1;

    seq_t product = (seq_t)1;
    idx_t temp = n;

    /* Iterate over decimal digits */
    while (temp > 0)
    {
        uint8_t digit = temp % 10;
        if (digit != 0)
            product *= (seq_t)digit;
        temp /= 10;
    }

    return product;
}
