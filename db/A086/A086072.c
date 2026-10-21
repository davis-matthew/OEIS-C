/*
 * A086072  Number of 1's in decimal expansion of triangular number n(n+1)/2.
 * https://oeis.org/A086072
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of decimal 1's in the triangular number n*(n+1)/2
 * Range:     u8 n <= 25, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the triangular number in seq_t to avoid overflow. */
    seq_t tri = (seq_t)n * ((seq_t)n + 1) / 2;

    /* Count the decimal digits equal to 1. */
    seq_t count = 0;
    seq_t x = tri;

    while (x > 0)
    {
        if (x % 10 == 1)
            count++;
        x /= 10;
    }

    return count;
}
