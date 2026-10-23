/*
 * A100817  Product of the digits of n, each doubled.
 * https://oeis.org/A100817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Π_{digits d of n} (2*d)
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t prod = (seq_t)1;
    idx_t m = n;

    while (m > 0) {
        idx_t d = m % 10;
        if (d == 0)
            return (seq_t)0;
        prod *= (seq_t)(2 * d);
        m /= 10;
    }

    return prod;
}
