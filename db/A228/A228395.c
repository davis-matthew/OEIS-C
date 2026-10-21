/*
 * A228395  The number of permutations of length n sortable by 3 prefix block
 *          transpositions.
 * https://oeis.org/A228395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + (1/24)*(3n^6 - 37n^5 + 184n^4 - 441n^3 + 509n^2 -
 *            218n)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N  = (seq_t)n;
    seq_t n2 = N * N;
    seq_t n3 = n2 * N;
    seq_t n4 = n3 * N;
    seq_t n5 = n4 * N;
    seq_t n6 = n5 * N;

    seq_t pos = ((seq_t)3 * n6) + ((seq_t)184 * n4) + ((seq_t)509 * n2);
    seq_t neg = ((seq_t)37 * n5) + ((seq_t)441 * n3) + ((seq_t)218 * N);

    seq_t num = pos - neg;
    seq_t res = num / 24 + 1;

    return res;
}
