/*
 * A094432  a(n) = rightmost term in M^n * [1 0 0]. M = the 3 X 3 stiffness
 *          matrix [1 -1 0 / -1 4 -3 / 0 -3 3].
 * https://oeis.org/A094432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=0, a(2)=3, for n≥3: a(n)=8*a(n-1)-9*a(n-2)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 0;           /* a(1) */
    if (n == 2) return 3;           /* a(2) */

    seq_t a_prev2 = 0;              /* a(n-2) */
    seq_t a_prev1 = 3;              /* a(n-1) */
    seq_t a_curr  = 0;              /* a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        a_curr = a_prev1 * 8 - a_prev2 * 9;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_curr;
}
