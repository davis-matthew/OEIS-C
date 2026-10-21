/*
 * A082791  Smallest k such that k*n begins with 2: a(n) = A082811(n)/n.
 * https://oeis.org/A082791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = smallest k such that k*n begins with 2
 * Range:     u8 n <= 63, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    for (seq_t k = 1; ; ++k) {
        seq_t prod = (seq_t)n * k;          /* k * n */
        seq_t d = prod;
        while (d >= 10) d /= 10;            /* find first digit */
        if (d == 2) return k;
    }
}
