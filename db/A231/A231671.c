/*
 * A231671  a(n) = Sum_{i=0..n} digsum_5(i)^4, where digsum_5(i) = A053824(i).
 * https://oeis.org/A231671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{i=0..n} digsum_5(i)^4
 * Range:     u8 n <= 3, u16 n <= 65, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* sum of digits of x in base 5 */
static inline unsigned int digsum5(idx_t x)
{
    unsigned int sum = 0;
    while (x > 0) {
        sum += (unsigned int)(x % 5);
        x /= 5;
    }
    return sum;
}

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t i = 0; i <= n; ++i) {
        unsigned int d  = digsum5(i);
        seq_t ds        = (seq_t)d;
        seq_t d2        = ds * ds;
        seq_t d4        = d2 * d2;
        total += d4;
    }
    return total;
}
