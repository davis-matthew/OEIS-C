/*
 * A171765  a(n) = 0 if n <= 10; for n >= 11, a(n) = product of digits of n.
 * https://oeis.org/A171765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 0 if n ≤ 10; otherwise product of decimal digits of n
 * Range:     u8 n <= 110, u16 n <= 110, u32 n <= 110, u64 n <= 110,
 *            u128 n <= 110, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 10) {
        return 0;
    }
    seq_t prod = 1;
    idx_t t = n;
    while (t > 0) {
        unsigned digit = t % 10;
        prod *= (seq_t)digit;
        t /= 10;
    }
    return prod;
}
