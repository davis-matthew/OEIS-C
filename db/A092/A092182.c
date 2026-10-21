/*
 * A092182  Figurate numbers based on the 600-cell (4-D polytope with
 *          Schlaefli symbol {3,3,5}).
 * https://oeis.org/A092182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*((145*n^3)-(280*n^2)+(179*n)-38)/6
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 99, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not part of the sequence, but safe guard  */

    seq_t sn  = (seq_t)n;          /* convert n once to seq_t */
    seq_t n2  = sn * sn;           /* n^2  */
    seq_t n3  = n2 * sn;           /* n^3  */

    /* numerator = n * (145*n^3 - 280*n^2 + 179*n - 38) */
    seq_t numerator =
        sn * ((seq_t)145 * n3
              - (seq_t)280 * n2
              + (seq_t)179 * sn
              - (seq_t)38);

    /* division by 6 is exact by definition of the sequence */
    return numerator / 6;
}
