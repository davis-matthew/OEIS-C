/*
 * A079503  a(n) = (n-1)^3*((n-2)^2 - 2*(n-3)).
 * https://oeis.org/A079503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)^3 * ((n-2)^2 - 2*(n-3)) (offset 1)
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 86, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Behaviour for n < 1 is undefined; return 0 safely */
        return 0;
    }

    /* Compute the linear terms in signed 64‑bit integers to handle
     * possible negative values for n < 3.  The values are small enough
     * that 64‑bit arithmetic cannot overflow. */
    int64_t t1 = (int64_t)n - 1;   /* n-1 */
    int64_t t2 = (int64_t)n - 2;   /* n-2 */
    int64_t t3 = (int64_t)n - 3;   /* n-3 */

    /* (n-1)^3  --> all positive, cast to seq_t */
    seq_t p1 = (seq_t)(t1 * t1 * t1);

    /* Inside the second factor: (n-2)^2 - 2*(n-3) */
    int64_t inside = t2 * t2 - 2 * t3;   /* fits in int64_t */
    seq_t p2 = (seq_t)inside;           /* cast to seq_t */

    /* Final product */
    return p1 * p2;
}
