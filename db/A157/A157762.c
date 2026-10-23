/*
 * A157762  a(n) = 15780962*n^2 - 5618000*n + 500001.
 * https://oeis.org/A157762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15780962*n^2 - 5618000*n + 500001
 * Range:     u8 none, u16 none, u32 n <= 16, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    return ((seq_t)15780962 * N * N
            - ((seq_t)5618000 * N)
            + (seq_t)500001);
}
