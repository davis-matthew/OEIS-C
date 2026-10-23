/*
 * A180191  Number of permutations of [n] having at least one succession. A
 *          succession of a permutation p is a position i such that
 *          p(i+1)-p(i) = 1.
 * https://oeis.org/A180191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! - d(n) - d(n-1) where d(j) is the j‑th derangement
 *            number, defined by d(0)=1, d(1)=0, d(j) = (j-1)*(d(j-1)+d(j-2))
 *            for j≥2. This implementation computes the factorial and the two
 *            last derangements in O(n) time for each call, with no dynamic
 *            allocation. It works for either fixed‑width bu...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base values for factorial and derangements. */
    seq_t fact = (seq_t)1;     /* 0! = 1, will become n!       */
    seq_t der0 = (seq_t)1;     /* d(0) */
    seq_t der1 = (seq_t)0;     /* d(1) */

    /* For n = 1 the loop is skipped; fact = 1, der0 = 1, der1 = 0. */
    for (idx_t i = (idx_t)2; i <= n; ++i) {
        fact = fact * (seq_t)i;                       /* factorial */
        seq_t new_der = ((seq_t)(i-1)) * (der1 + der0); /* d(i)   */
        der0 = der1;                                   /* shift */
        der1 = new_der;
    }
    /* now der1 = d(n), der0 = d(n-1) */
    return fact - der1 - der0;
}
