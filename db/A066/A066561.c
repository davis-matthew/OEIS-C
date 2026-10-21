/*
 * A066561  a(n) is the smallest triangular number divisible by n.
 * https://oeis.org/A066561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest triangular number divisible by n
 *            Definition: a(n) = min{ T_m : T_m = m*(m+1)/2 divisible by n }
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t nn = n;
    for (idx_t m = 1; ; ++m) {
        idx_t tri = m * (m + 1) / 2;           /* fitting in idx_t for n <= 10000 */
        if (tri % nn == 0)
            return (seq_t)tri;
    }
}
