/*
 * A028668  Pseudo Galois numbers for d=6.
 * https://oeis.org/A028668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6^n * product_{k=0}^{n-1} (6^n - 6^k)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    seq_t result = (seq_t)1;      /* running product a(n) */
    seq_t pow6   = (seq_t)1;      /* 6^i in the loop */

    for (idx_t i = 1; i <= n; ++i)
    {
        pow6 = pow6 * (seq_t)6;                 /* pow6 = 6^i   */
        seq_t term = pow6 * (pow6 - (seq_t)1);  /* 6^i * (6^i-1) */
        result = result * term;                 /* a(i) = a(i-1) * term */
    }

    return result;
}
