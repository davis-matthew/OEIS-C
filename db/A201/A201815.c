/*
 * A201815  Number of arrays of 7 integers in -n..n with sum zero and equal
 *          numbers of elements greater than zero and less than zero.
 * https://oeis.org/A201815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 77*n^5 + 175*n^3 + 140*n + 1
 * Range:     u8 none, u16 n <= 3, u32 n <= 35, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t np = (seq_t)n;
    seq_t np2 = np * np;
    seq_t np3 = np2 * np;
    seq_t np5 = np3 * np2;
    seq_t result = ((seq_t)77 * np5) + ((seq_t)175 * np3) + ((seq_t)140 * np) + 1;
    return result;
}
