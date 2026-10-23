/*
 * A121723  a(n) = A098916(n+2) + (1-n) * A067318(n).
 * https://oeis.org/A121723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * (n-1) * Sum_{i=1..n} (1/i) (OEIS A121723)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)                /* safety for n < 1 */
        return 0;

    /* factorial so far (i-1)! when loop index is i */
    seq_t fact = 1;

    /* S_i = i! * H_i  (i.e. the sum part) */
    seq_t S = 1;                /* S_1 = 1! * H_1 = 1 */

    if (n == 1)                /* a(1) = 0 because n-1 = 0 */
        return 0;

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t prev_fact = fact;          /* (i-1)!   */
        fact = fact * (seq_t)i;          /* i!       */
        S = (seq_t)i * S + prev_fact;    /* S_i = i*S_{i-1} + (i-1)! */
    }

    /* a(n) = (n-1) * S_n */
    return (seq_t)(n - 1) * S;
}
