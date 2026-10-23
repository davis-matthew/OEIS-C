/*
 * A133694  a(n) = (3*n^2 + 3*n - 4)/2.
 * https://oeis.org/A133694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n*n + 3*n - 4)/2
 * Range:     u8 n <= 12, u16 n <= 208, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS entry: A133694 */
/*  a(1) = 1, a(2) = 7, a(3) = 16, ... */

seq_t a(idx_t n)
{
    /* Undefined for n < 1, but returning 0 keeps the function well‑defined. */
    if (n == 0) return 0;

    seq_t nn = (seq_t)n;

    /* Compute n*(n+1)/2 first to keep intermediate values small */
    seq_t tri = nn * (nn + 1);
    tri /= 2;          /* now tri = n*(n+1)/2 = triangular number */

    tri *= 3;          /* 3 * triangular number = 3*A000217(n) */
    tri -= 2;          /* final formula: 3*A000217(n) - 2 */

    return tri;
}
