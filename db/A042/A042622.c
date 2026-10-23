/*
 * A042622  Numerators of continued fraction convergents to sqrt(840).
 * https://oeis.org/A042622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 58*a(n-2) - a(n-4) with initial values a(0) = 28, a(1) =
 *            29, a(2) = 1652, a(3) = 1681 (same recurrence holds for both
 *            even and odd subsequences)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special cases */
    if (n == 0) return (seq_t)28;
    if (n == 1) return (seq_t)29;

    /* For even n, treat the subsequence b_k = a(2k):
       b_0 = 28, b_1 = 1652, b_k = 58*b_{k-1} - b_{k-2} */
    if ((n & 1) == 0) {
        idx_t k = n >> 1;          /* k = n/2  (k >= 1) */
        seq_t prev2 = (seq_t)28;   /* b_0    */
        seq_t prev1 = (seq_t)1652; /* b_1    */
        if (k == 1) return prev1;  /* a(2)   */
        for (idx_t i = 2; i <= k; ++i) {
            seq_t curr = (seq_t)58 * prev1 - prev2;
            prev2 = prev1;
            prev1 = curr;
        }
        return prev1;              /* a(2k)  */
    }

    /* For odd n, treat the subsequence c_k = a(2k+1):
       c_0 = 29, c_1 = 1681, c_k = 58*c_{k-1} - c_{k-2} */
    {
        idx_t k = n >> 1;          /* k = floor(n/2) (k >= 1 for n>=3) */
        seq_t prev2 = (seq_t)29;   /* c_0    */
        seq_t prev1 = (seq_t)1681; /* c_1    */
        if (k == 1) return prev1;  /* a(3)   */
        for (idx_t i = 2; i <= k; ++i) {
            seq_t curr = (seq_t)58 * prev1 - prev2;
            prev2 = prev1;
            prev1 = curr;
        }
        return prev1;              /* a(2k+1)*/
    }
}
