/*
 * A220660  Irregular table, where the n-th row consists of numbers 0..(n!-1).
 * https://oeis.org/A220660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = n - A007489(A084556(n)-1) - 1. Equivalent iterative
 *            implementation: a(n) = n - (1 + ∑_{k=1}^{r-1} k!) where r is the
 *            smallest integer with ∑_{k=1}^r k! ≥ n. The value a(n) is the
 *            0‑based rank of n within the block of size r! that starts at
 *            index 1 + ∑_{k=1}^{r-1} k!
 * Range:     u8 n <= 220, u16 n <= 5913, u32 n <= 5913, u64 n <= 5913,
 *            u128 n <= 5913, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* No external includes needed; seq_t and idx_t are provided by the build. */
seq_t a(idx_t n)
{
    /* cumulative sum of factorials before the current block */
    seq_t cum = 0;
    /* factorial of the current block size (starts at 1! = 1) */
    seq_t fact = 1;
    /* block counter (starts at 1) */
    idx_t r = 1;

    /* Find the smallest r such that cumulative + fact >= n */
    while (cum + fact < n) {
        cum  += fact;                 /* accumulate previous block size   */
        fact *= (seq_t)(r + 1);       /* move to next factorial (= (r+1)!) */
        ++r;                          /* advance block counter            */
    }

    /* a(n) = n - (cum + 1)  (cum is sum_{k=1}^{r-1} k!) */
    return (seq_t)n - (cum + 1);
}
