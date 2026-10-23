/*
 * A138772  Number of entries in the second cycles of all permutations of
 *          {1,2,...,n}; each cycle is written with the smallest element first
 *          and cycles are arranged in increasing order of their first
 *          elements.
 * https://oeis.org/A138772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)*a(n-1) + (n-2)! (a(1)=0, a(2)=1)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 2) {
        /* for n = 1 or 0 */
        return (seq_t)0;
    }
    seq_t a_prev = (seq_t)1;   /* a(2) */
    seq_t fact   = (seq_t)1;   /* (k-2)! initially 1! = 1 */
    for (idx_t k = 3; k <= n; ++k) {
        seq_t term  = fact;                               /* (k-2)!   */
        a_prev = ((seq_t)(k + 1) * a_prev) + term;          /* a(k)     */
        fact *= (seq_t)(k - 1);                            /* update to (k-1)! for next iteration */
    }
    return a_prev;
}
