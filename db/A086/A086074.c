/*
 * A086074  Number of 3's in decimal expansion of triangular number n(n+1)/2.
 * https://oeis.org/A086074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of decimal digits '3' in n*(n+1)/2
 * Range:     u8 n <= 15, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the triangular number as seq_t */
    seq_t tri = (seq_t)n * ((seq_t)n + (seq_t)1);
    tri /= (seq_t)2;

    /* Count the number of '3' digits in tri's decimal expansion */
    seq_t count = (seq_t)0;

    while (tri != (seq_t)0) {
        seq_t rem = tri % (seq_t)10;
        if (rem == (seq_t)3) {
            count++;
        }
        tri = tri / (seq_t)10;
    }

    return count;
}
