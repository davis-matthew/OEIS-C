/*
 * A218059  Number of n X 3 arrays of the minimum value of corresponding
 *          elements and their horizontal or antidiagonal neighbors in a
 *          random 0..1 n X 3 array.
 * https://oeis.org/A218059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 3*a(n-2) - 2*a(n-3), a(1) = 4, a(2) = 18, a(3)
 *            = 74
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)18;
    if (n == 3) return (seq_t)74;

    seq_t prev3 = (seq_t)4;   /* a(1) */
    seq_t prev2 = (seq_t)18;  /* a(2) */
    seq_t prev1 = (seq_t)74;  /* a(3) */

    seq_t cur = 0;
    for (idx_t i = 4; i <= n; ++i) {
        seq_t t1 = (seq_t)5 * prev1;
        seq_t t2 = (seq_t)3 * prev2;
        seq_t t3 = (seq_t)2 * prev3;
        cur = t1 - t2 - t3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
