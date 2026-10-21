/*
 * A066577  a(n) = floor(n/(product of nonzero digits of n)).
 * https://oeis.org/A066577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(n / (product of non-zero digits of n))
 * Range:     u8 n <= 285, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t temp = n;
    seq_t prod = 1;
    while (temp > 0) {
        int d = temp % 10;
        if (d != 0)
            prod *= (seq_t)d;
        temp /= 10;
    }
    return (seq_t)n / prod;
}
