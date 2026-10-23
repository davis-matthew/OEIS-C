/*
 * A122046  Partial sums of floor(n^2/8).
 * https://oeis.org/A122046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2*n^3 + 3*n^2 - 8*n + 3)/48)
 * Range:     u8 n <= 17, u16 n <= 115, u32 n <= 4688, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Partial sums of floor(k^2/8) for k = 0..n */

seq_t a(idx_t n)
{
    /* Use unsigned __int128 to avoid overflow of the cubic term. */
    unsigned __int128 un = (unsigned __int128)n;

    /* Compute numerator: 2*n^3 + 3*n^2 - 8*n + 3 */
    unsigned __int128 num = 2 * un * un * un
                          + 3 * un * un
                          - 8 * un
                          + 3;

    /* Divide by 48 (integer division gives the floor). */
    unsigned __int128 res = num / 48;

    return (seq_t)res;
}
