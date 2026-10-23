/*
 * A011932  a(n) = floor( n*(n-1)*(n-2)*(n-3)/22 ).
 * https://oeis.org/A011932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n-1)*(n-2)*(n-3)/22)
 * Range:     u8 n <= 10, u16 n <= 36, u32 n <= 555, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Values for n < 4 are zero by definition. */
    if (n < 4) {
        return 0;
    }

    /* Use an intermediate 128‑bit type to avoid overflow when multiplying
       the four successive terms.  This works for both 64‑bit and 128‑bit
       builds, since casting from __uint128_t to seq_t truncates if the
       result does not fit, as required by the problem. */
    unsigned __int128 prod = (unsigned __int128)n;
    prod *= (n - 1);
    prod *= (n - 2);
    prod *= (n - 3);

    /* Integer division truncates towards zero, which equals floor
       for non‑negative numbers. */
    prod /= 22;

    return (seq_t)prod;
}
