/*
 * A166669  Totally multiplicative sequence with a(p) = 3p-2 for prime p.
 * https://oeis.org/A166669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = \prod_{p^e||n} (3p-2)^e
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return n == 1 ? (seq_t)1 : (seq_t)0;   /* n == 1 gives a(1) = 1 */

    seq_t result = 1;
    uint64_t m = n;

    for (uint64_t p = 2; p <= m / p; ++p)
    {
        if (m % p == 0)
        {
            int exp = 0;
            while (m % p == 0)
            {
                m /= p;
                ++exp;
            }
            seq_t base = (seq_t)3 * (seq_t)p - 2;
            seq_t power = 1;
            for (int i = 0; i < exp; ++i)
                power *= base;
            result *= power;
        }
    }

    /* Remaining prime factor (exponent 1) */
    if (m > 1)
    {
        seq_t base = (seq_t)3 * (seq_t)m - 2;
        result *= base;
    }

    return result;
}
