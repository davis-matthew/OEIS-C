/*
 * A014144  Apply partial sum operator twice to factorials.
 * https://oeis.org/A014144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * (sum_{k=0}^{n-1} k!) - n! + 1
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the second partial sum of factorials (OEIS A014144). */

seq_t a(idx_t n)
{
    seq_t fact = (seq_t)1;   /* current k!   (k starts at 0) */
    seq_t sum  = (seq_t)0;   /* partial sum of factorials */

    for (idx_t k = 0; k < n; ++k) {
        sum += fact;
        fact *= (seq_t)(k + 1);  /* move to (k+1)! for next iteration */
    }

    return (seq_t)n * sum - fact + (seq_t)1;   /* a(n)   */
}
