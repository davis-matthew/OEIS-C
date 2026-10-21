/*
 * A204257  Matrix given by f(i,j)=1+[(i+2j) mod 3], by antidiagonals.
 * https://oeis.org/A204257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the value 1+((i+2*j) mod 3) where (i,j) are the
 *            coordinates * of the n‑th element in the antidiagonal traversal
 *            of the infinite matrix * f(i,j)=1+[(i+2j) mod 3], starting with
 *            (1,1) as n=1. The offset is 1
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Guard against n==0, though the OEIS offset starts at 1. */
    if (n == 0) return (seq_t)0;

    /* Find the diagonal number s such that (s-1)*s/2 >= n.
     * (s-1)*s/2 is the triangular number T(s-1).  s >= 2.   */
    idx_t low = 2;
    idx_t high = 2;
    /* Increase high until it satisfies the inequality.  The product
     * (high-1)*high/2 will not overflow for any n that fits in idx_t. */
    while (((high - 1) * high) / 2 < n) {
        high <<= 1; /* double high */
    }

    /* Binary search for minimal s. */
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if (((mid - 1) * mid) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t s = low; /* diagonal number (i+j).  s >= 2. */

    /* Count of terms before this diagonal.  This is T(s-2). */
    idx_t before = ((s - 2) * (s - 1)) / 2;

    /* Position within the diagonal (1 … s-1). */
    idx_t pos = n - before;   /* 1 ≤ pos ≤ s-1 */

    /* Coordinates: i = pos, j = s - pos. */
    idx_t i = pos;
    idx_t j = s - pos;

    /* Compute f(i,j) = 1 + ((i + 2*j) mod 3). */
    seq_t res = (seq_t)1
              + (((seq_t)i + ( (seq_t)2 * (seq_t)j )) % (seq_t)3);

    return res;
}
