/*
 * A159941  Number of trisubstituted linear alkanes of composition C_n
 *          H_(2n-1) XYZ.
 * https://oeis.org/A159941
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n^3 - 18*n^2 + 38*n - 28 (for n >= 2)
 * Range:     u8 n <= 5, u16 n <= 26, u32 n <= 1025, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    seq_t un = n;                      /* cast to seq_t once */
    seq_t t1 = ((seq_t)4 * un * un * un);
    seq_t t2 = ((seq_t)18 * un * un);
    seq_t t3 = ((seq_t)38 * un);
    seq_t result = t1 - t2 + t3 - (seq_t)28;
    return result;
}
