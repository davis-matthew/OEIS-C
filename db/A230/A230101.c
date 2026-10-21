/*
 * A230101  a(n) = product of n and all its nonzero digits.
 * https://oeis.org/A230101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = n * Π_{d∈digits(n), d≠0} d
 * Range:     u8 n <= 25, u16 n <= 378, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = 1;
    idx_t t = n;

    /* If n is 0, prod stays 1 and the result is 0. */
    while (t > 0) {
        unsigned d = t % 10;
        if (d != 0)
            prod *= (seq_t)d;
        t /= 10;
    }

    return (seq_t)n * prod;
}
