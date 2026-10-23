/*
 * A033440  Number of edges in 8-partite Turán graph of order n.
 * https://oeis.org/A033440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} floor(7*i/8) This follows from the OEIS
 *            description: f(n)=∑_{i=1}^{n}⌊7i/8⌋
 * Range:     u8 n <= 24, u16 n <= 387, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        /* compute floor(7*i/8) without overflowing small seq_t
           (i/8)*7 + (i%8 * 7)/8  guarantees all intermediates fit */
        seq_t term = ((seq_t)(i / 8) * 7)
                   + ((seq_t)((i % 8) * 7) / 8);
        sum += term;
    }
    return sum;
}
