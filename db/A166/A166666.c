/*
 * A166666  Totally multiplicative sequence with a(p) = 8p+1 for prime p.
 * https://oeis.org/A166666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e || n} (8p + 1)^e (totally multiplicative)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1UL)  /* offset 1 – a(1)=1, a(0) unused */
        return (n == 1) ? 1 : 0;

    idx_t tmp = n;
    seq_t result = (seq_t)1;

    /* trial division */
    for (idx_t p = (idx_t)2; p * p <= tmp; ++p)
    {
        if (tmp % p != (idx_t)0)
            continue;

        idx_t e = (idx_t)0;
        while (tmp % p == (idx_t)0)
        {
            tmp /= p;
            ++e;
        }

        seq_t base = (seq_t)(8 * (seq_t)p + (seq_t)1);
        for (idx_t i = (idx_t)0; i < e; ++i)
            result *= base;
    }

    /* remaining prime factor > 1 */
    if (tmp > (idx_t)1)
    {
        seq_t base = (seq_t)(8 * (seq_t)tmp + (seq_t)1);
        result *= base;
    }

    return result;
}
