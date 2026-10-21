/*
 * A079069  a(n) = a(n-2) * a(n-3) + a(n-3), n>3. a(1) = a(2) = a(3) = 1.
 * https://oeis.org/A079069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2)*a(n-3) + a(n-3), with a(1)=a(2)=a(3)=1
 * Range:     u8 n <= 11, u16 n <= 13, u32 n <= 16, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1; no defined value below 1 */
    if (n <= 3) return (seq_t)1;    /* initial values */

    /* Sliding window: prev2 = a(i-3), prev1 = a(i-2), curr = a(i-1) */
    seq_t prev2 = 1;   /* a(1) */
    seq_t prev1 = 1;   /* a(2) */
    seq_t curr  = 1;   /* a(3) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = prev1 * prev2 + prev2;  /* a(i) = a(i-2)*a(i-3)+a(i-3) */
        prev2 = prev1;
        prev1 = curr;
        curr  = next;
    }
    return curr;
}
