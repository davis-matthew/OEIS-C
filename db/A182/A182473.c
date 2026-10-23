/*
 * A182473  Number of partitions of n into exactly 2 different parts with
 *          distinct multiplicities.
 * https://oeis.org/A182473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of partitions of n into exactly 2 different parts
 *            with distinct multiplicities
 * Range:     u8 n <= 41, u16 n <= 1799, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4) return 0;          /* first non‑zero value occurs at n = 4 */

    seq_t count = 0;

    /* a : smaller part value, must be at least 1 and strictly less than the other part */
    for (idx_t a = 1; a <= n / 2; ++a)
    {
        /* j : multiplicity of the smaller part a, j >= 1 */
        for (idx_t j = 1; ; ++j)
        {
            idx_t r = n - a * j;                     /* remaining sum that must be a multiple of the larger part */
            if (r <= 0) break;                       /* no further j possible */
            if (r < a + 1) break;                    /* smallest possible divisor > a is a+1 */

            /* enumerate all divisors of r > a */
            for (idx_t d = 1; d * d <= r; ++d)
            {
                if (r % d != 0) continue;
                idx_t d1 = d;
                idx_t d2 = r / d;

                /* divisor d1 > a gives larger part = d1, multiplicity = d2 */
                if (d1 > a)
                {
                    idx_t k = d2;          /* multiplicity of the larger part */
                    if (j != k) ++count;   /* multiplicities must differ */
                }

                /* divisor d2 > a (and distinct from d1) gives larger part = d2, multiplicity = d1 */
                if (d2 > a && d2 != d1)
                {
                    idx_t k = d1;
                    if (j != k) ++count;
                }
            }
        }
    }

    return count;
}
