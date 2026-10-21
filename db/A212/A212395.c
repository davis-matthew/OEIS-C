/*
 * A212395  Number of move operations required to sort all permutations of [n]
 *          by insertion sort.
 * https://oeis.org/A212395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(0) = 0 * a(n) = a(n-1)*n + (n-1)! * (n-1)*(n+4)/2 for n > 0
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t aPrev   = 0;   /* a(n-1) */
    seq_t factPrev = 1;  /* (n-1)!  */
    for (idx_t i = 1; i <= n; ++i) {
        /* factor = ((i-1)*(i+4))/2  is always integer */
        seq_t factor = ((seq_t)(i - 1) * ((seq_t)i + 4)) / 2;
        seq_t term   = factPrev * factor;
        seq_t aNew   = aPrev * (seq_t)i + term;
        aPrev = aNew;
        factPrev *= i;           /* (i)! becomes next factorial */
    }
    return aPrev;
}
