/*
 * A174746  x-values in the solution to x^2-31*y^2=1.
 * https://oeis.org/A174746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3040*a(n-1) – a(n-2), a(1) = 1, a(2) = 1520
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the two initial values directly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1520;

    /* iterate from the third term onward, keeping only the two most recent values */
    seq_t prev2 = (seq_t)1;      /* a(1) */
    seq_t prev1 = (seq_t)1520;   /* a(2) */
    seq_t cur = 0;

    for (idx_t i = 3; i <= n; ++i) {
        cur    = (seq_t)3040 * prev1 - prev2;  /* a(i) = 3040*a(i-1) – a(i-2) */
        prev2  = prev1;
        prev1  = cur;
    }
    return prev1;   /* prev1 now holds a(n) */
}
