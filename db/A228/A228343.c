/*
 * A228343  The number of ordered trees with bicolored single edges on the
 *          boundary.
 * https://oeis.org/A228343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 2, a(2) = 5. For n ≥ 3: a(n) = [6*(n² – 2)*a(n
 *            – 1) – 4*n*(2*n – 1)*a(n – 2)] / [(n + 3)*(n – 2)]
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)2;
    if (n == (idx_t)2) return (seq_t)5;

    seq_t prev2 = (seq_t)2;   /* a(1) */
    seq_t prev1 = (seq_t)5;   /* a(2) */

    for (idx_t i = (idx_t)3; i <= n; ++i) {
        seq_t term1 = 6 * ((seq_t)i * (seq_t)i - (seq_t)2) * prev1;
        seq_t term2 = 4 * (seq_t)i *
                      (((seq_t)2 * (seq_t)i) - (seq_t)1) * prev2;
        seq_t denom = ((seq_t)i + (seq_t)3) *
                       ((seq_t)i - (seq_t)2);
        seq_t curr = (term1 - term2) / denom;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
