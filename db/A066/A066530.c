/*
 * A066530  Expansion of (1+x+x^3)/((1-x)*(1-x^4)).
 * https://oeis.org/A066530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((3*n + 5)/4)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a wider intermediate type to avoid overflow for large n. */
    unsigned long long temp = 3ULL * (unsigned long long)n + 5ULL;
    return (seq_t)(temp / 4ULL);
}
