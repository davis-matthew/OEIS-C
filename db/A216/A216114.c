/*
 * A216114  The Wiener index of a link of n fullerenes C_{20} (see the
 *          Ghorbani and Hosseinzadeh reference).
 * https://oeis.org/A216114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 100*n*(2*n*n + 6*n - 3)
 * Range:     u8 none, u16 n <= 6, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = n;
    seq_t tmp = 2*nn*nn + 6*nn - 3;
    return 100*nn*tmp;
}
