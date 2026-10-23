/*
 * A035471  Coordination sequence for lattice D*_4 (with edges defined by l_1
 *          norm = 1).
 * https://oeis.org/A035471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; for n > 0, a(n) = (16·n³ + 8·n)/3
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 765, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    seq_t nn = (seq_t)n;
    /* compute 16 * n^3 + 8 * n */
    seq_t val = (seq_t)16 * nn * nn * nn + (seq_t)8 * nn;
    val /= (seq_t)3;
    return val;
}
