/*
 * A083332  a(n) = 10*a(n-2) - 16*a(n-4) for n > 3, a(0) = 1, a(1) = 5, a(2) =
 *          14, a(3) = 34.
 * https://oeis.org/A083332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-2) - 16*a(n-4) with initial values a(0)=1, a(1)=5,
 *            a(2)=14, a(3)=34 This implementation computes the subsequences
 *            for even and odd indices separately, each being a second‑order
 *            linear recurrence. Complexity: O(n) per call, constant extra
 *            space
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial values explicitly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)14;
    if (n == 3) return (seq_t)34;

    /* Even indices : a(2k) = 10*a(2k-2) - 16*a(2k-4) */
    if ((n & 1) == 0) {
        idx_t k = n >> 1;               /* k = n/2   */
        seq_t e_prev2 = (seq_t)1;        /* a(0)     */
        seq_t e_prev1 = (seq_t)14;       /* a(2)     */
        if (k == 0) return e_prev2;     /* Only for n=0 which is already handled */
        for (idx_t i = 2; i <= k; ++i) {
            seq_t e_curr = (seq_t)10 * e_prev1 - (seq_t)16 * e_prev2;
            e_prev2 = e_prev1;
            e_prev1 = e_curr;
        }
        return e_prev1;
    }

    /* Odd indices : a(2k+1) = 10*a(2k-1) - 16*a(2k-3) */
    else {
        idx_t k = (n - 1) >> 1;         /* k = (n-1)/2 */
        seq_t o_prev2 = (seq_t)5;        /* a(1)     */
        seq_t o_prev1 = (seq_t)34;       /* a(3)     */
        if (k == 0) return o_prev2;     /* Only for n=1 which is already handled */
        for (idx_t i = 2; i <= k; ++i) {
            seq_t o_curr = (seq_t)10 * o_prev1 - (seq_t)16 * o_prev2;
            o_prev2 = o_prev1;
            o_prev1 = o_curr;
        }
        return o_prev1;
    }
}
