/*
 * A215096  a(0)=0, a(1)=1, a(n) = n! - a(n-2).
 * https://oeis.org/A215096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! - a(n-2), with a(0) = 0, a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t fact = 1;      /* start with 0! = 1 */
    seq_t a_prev2 = 0;   /* a(0) */
    seq_t a_prev1 = 1;   /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        fact = fact * (seq_t)i;             /* fact = i!   */
        seq_t cur = fact - a_prev2;         /* a(i) = i! - a(i-2) */
        a_prev2 = a_prev1;                  /* shift for next step */
        a_prev1 = cur;
    }

    return a_prev1;
}
