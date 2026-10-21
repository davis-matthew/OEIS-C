/*
 * A055533  Number of labeled order relations on n nodes in which longest
 *          chain has n-1 nodes.
 * https://oeis.org/A055533
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * (n-1)^2 / 2 (n ≥ 2)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;            /* values below the offset are unused */

    seq_t fact = 1;                 /* n!  */
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    seq_t base = (seq_t)(n - 1);
    base *= base;                   /* (n-1)^2   */
    fact *= base;                   /* n! * (n-1)^2   */
    fact /= (seq_t)2;               /* divide by 2   */

    return fact;
}
