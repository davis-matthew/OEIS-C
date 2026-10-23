/*
 * A152211  a(n) = n * sigma_0(n) + sigma_1(n).
 * https://oeis.org/A152211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n * sigma_0(n) + sigma_1(n)
 * Range:     u8 n <= 29, u16 n <= 1679, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Where sigma_0(n) is the number of divisors of n, */
/* and sigma_1(n) is the sum of all divisors of n.   */
/* This implementation works for all supported builds, */
/* including the arbitrary‑precision C++ variant.      */
seq_t a(idx_t n)
{
    /* n = 0 can never be requested (offset = 1), but handle safely. */
    if (n == 0)
        return 0;

    seq_t sigma0 = 0;   /* divisor count */
    seq_t sigma1 = 0;   /* divisor sum   */

    /* Enumerate divisors in pairs (d, n/d) to keep O(sqrt(n)) time. */
    for (idx_t d = 1; d <= n / d; ++d)
    {
        if (n % d == 0)
        {
            /* divisor d */
            ++sigma0;
            sigma1 += (seq_t)d;

            idx_t e = n / d;
            if (e != d)
            {
                /* complementary divisor e, distinct from d */
                ++sigma0;
                sigma1 += (seq_t)e;
            }
        }
    }

    /* n is idx_t; cast to seq_t before multiplication to avoid
       implicit narrowing in arbitrary‑precision builds.                */
    return ((seq_t)n) * sigma0 + sigma1;
}
