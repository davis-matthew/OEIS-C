/*
 * A079505  The last number for which a determinant of base-n numbers is
 *          nonzero.
 * https://oeis.org/A079505
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = n^3 - n + 1 (exception: a(2) = 14)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1625, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)            /* undefined below offset; value irrelevant */
        return (seq_t)0;
    if (n == 2)           /* special case */
        return (seq_t)14;

    seq_t nn = (seq_t)n;          /* promote to seq_t for safe arithmetic */
    seq_t n_sq = nn * nn;         /* n^2 */
    seq_t n_cub = n_sq * nn;      /* n^3 */
    seq_t result = n_cub - nn + (seq_t)1; /* n^3 - n + 1 */
    return result;
}
