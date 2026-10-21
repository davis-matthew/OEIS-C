/*
 * A220400  Number of ways to write n as sum of at least 2 consecutive odd
 *          positive integers.
 * https://oeis.org/A220400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = count of solutions (m,k) with m≥2, k odd>0 such that n =
 *            m * (k + m - 1) (sum of at least two consecutive odd positive
 *            integers)
 * Range:     u8 n <= 9283, u16 n <= 9283, u32 n <= 9283, u64 n <= 9283,
 *            u128 n <= 9283, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    if (n < 2)
        return 0;

    /* For all m where m*m ≤ n the value k = n/m - m + 1 will be positive.
       We test each divisor m of n, compute the corresponding k,
       and increment the counter when k is a positive odd integer. */
    for (idx_t m = 2; m * m <= n; ++m)
    {
        if (n % m == 0)
        {
            idx_t q = n / m;           /* q = k + m - 1          */
            idx_t k = q - m + 1;       /* start of the odd run   */
            if ((k & 1UL) == 1UL)      /* k must be odd          */
                ++count;
        }
    }
    return count;
}
