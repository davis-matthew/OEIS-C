/*
 * A062331  a(n) is the product of the sum and the product of the digits of n
 *          (0 is not to be considered a factor in the product).
 * https://oeis.org/A062331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = (sum of decimal digits of n) * (product of non‑zero
 *            digits of n)
 * Range:     u8 n <= 37, u16 n <= 3998, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t prod = 1;
    idx_t tmp = n;
    while (tmp != 0) {
        unsigned d = tmp % 10;
        sum += d;
        if (d != 0) {
            prod *= (seq_t)d;
        }
        tmp /= 10;
    }
    return sum * prod;
}
