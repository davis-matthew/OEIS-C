/*
 * A054558  Number of labeled pure 2-complexes on n nodes (0-simplexes) with 5
 *          2-simplexes and 9 1-simplexes.
 * https://oeis.org/A054558
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(n-2)*(n-3)*(n-4)*(n*n + n + 150)/144
 * Range:     u8 none, u16 n <= 5, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Sequence is undefined below offset 5; return 0 for n < 5 */
    if (n < 5) return (seq_t)0;

    seq_t nn  = (seq_t)n;
    seq_t t1  = nn * ((seq_t)(n-1));
    seq_t t2  = t1 * ((seq_t)(n-2));
    seq_t t3  = t2 * ((seq_t)(n-3));
    seq_t t4  = t3 * ((seq_t)(n-4));

    seq_t s   = nn * nn + nn + (seq_t)150;  /* n^2 + n + 150 */
    seq_t prod = t4 * s;

    const seq_t denom = (seq_t)144;
    return prod / denom;
}
