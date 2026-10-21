/*
 * A086075  Number of 4's in decimal expansion of triangular number n(n+1)/2.
 * https://oeis.org/A086075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of decimal digits equal to 4 in the triangular
 *            number n*(n+1)/2
 * Range:     u8 n <= 23, u16 n <= 817, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tri = (seq_t)n * ((seq_t)n + 1) / 2;   /* triangular number */
    seq_t count = 0;
    while (tri > 0) {
        seq_t digit = tri % 10;                  /* least significant decimal digit */
        if (digit == 4)
            count++;
        tri /= 10;                                /* discard the processed digit */
    }
    return count;
}
