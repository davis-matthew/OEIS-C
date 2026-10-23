/*
 * A164486  Duplicate of A084684.
 * https://oeis.org/A164486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 1 + n*(n+1)/2 + ((n-1)/2)*(n/2) for n ≥ 1; a(0)=1
 * Range:     i8 n <= 12, i16 n <= 23, i32 n <= 23, i64 n <= 23, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t nn = (seq_t)n;
    seq_t term1 = nn * (nn + 1) / 2;
    seq_t term2 = ((seq_t)(n - 1) / 2) * ((seq_t)n / 2);
    return (seq_t)1 + term1 + term2;
}
