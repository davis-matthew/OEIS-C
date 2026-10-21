/*
 * A216665  Triangular array read by rows: T(n,k) is the number of partitions
 *          of n into k parts of 2 different sizes; n>=3, 2<=k<=n-1.
 * https://oeis.org/A216665
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) = number of partitions of i into j parts of *
 *            exactly two distinct sizes, read by rows of the triangle *
 *            (offset 3)
 * Range:     u8 n <= 9691, u16 n <= 9691, u32 n <= 9691, u64 n <= 9691,
 *            u128 n <= 9691, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utility: integer square root (floor) for 64‑bit unsigned numbers. */
static idx_t isqrt_u64(idx_t v)
{
    idx_t low = 0, high = v;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        if (mid == 0) { low = 1; continue; }
        if (mid > v / mid) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    /* n is the index in the flattened triangle starting from 3. */
    if (n < 3) return 0;

    /* Step 1: map the flat index to row N and column k. */
    idx_t r = n - 3;                      /* zero‑based position in the
                                            * flattened array.      */
    idx_t s = isqrt_u64(1 + 8 * r);        /* floor sqrt(1+8r)         */
    idx_t N = ((3 + s) >> 1) + 1;         /* row number (N >= 3)       */
    /* terms up to the previous row:
     * t_prev = (N-3)*(N-2)/2
     */
    idx_t t_prev = ((N - 3) * (N - 2)) >> 1;
    idx_t c = r - t_prev;                 /* position inside the row   */
    idx_t k = c + 2;                      /* k ranges from 2 to N-1    */

    /* Step 2: compute T(N,k). */
    seq_t total = 0;
    for (idx_t x = 1; x <= k - 1; ++x) {
        idx_t y = k - x;
        idx_t amax = (N - 1) / k;          /* a < N/K   */
        for (idx_t a = 1; a <= amax; ++a) {
            idx_t rem = N - a * x;
            if (rem == 0) break;
            if ((rem % y) == 0) {
                idx_t b = rem / y;
                if (b > a) {
                    total += (seq_t)1;
                }
            }
        }
    }
    return total;
}
