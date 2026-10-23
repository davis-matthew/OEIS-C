/*
 * A176732  a(n) = (n+5)*a(n-1) + (n-1)*a(n-2), a(-1)=0, a(0)=1.
 * https://oeis.org/A176732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+5)*a(n-1) + (n-1)*a(n-2) Initial values: a(0)=1,
 *            a(1)=6 (Sequence A176732)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;

    seq_t a_prev2 = (seq_t)1;      /* a(0) */
    seq_t a_prev1 = (seq_t)6;      /* a(1) */
    seq_t a_curr;

    for (idx_t i = 2; i <= n; ++i) {
        /* compute a(i) = (i+5)*a_prev1 + (i-1)*a_prev2 */
        a_curr = (seq_t)(i + 5) * a_prev1 + (seq_t)(i - 1) * a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_prev1;
}
