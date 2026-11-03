/*
 * A109496  a(n) is the first integer k for which b(b(...b(n)...))=5, where
 *          the transformation b(m), the m-th term of A008474, is taken k-1
 *          times.
 * https://oeis.org/A109496
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal k such that b^{k-1}(n)=5, where * b(m) = (sum of
 *            distinct prime factors of m) + (sum of their exponents)
 * Range:     u8 n <= 109, u16 n <= 109, u32 n <= 109, u64 n <= 109,
 *            u128 n <= 109, big any n
 * Validated: partial, pass, 105 terms through n = 109 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute b(m) for a given integer m.  The function works for all
 * values that occur in the chains for n up to 10000 and therefore
 * fits easily in seq_t. */
static seq_t b_func(seq_t m)
{
    /* Use 64-bit arithmetic for the factorisation. */
    uint64_t x = (uint64_t)m;
    uint64_t sum_p = 0, sum_e = 0;

    /* Factorise x by trial division. */
    for (uint64_t p = 2; p * p <= x; ++p)
    {
        if (x % p == 0)
        {
            uint64_t e = 0;
            sum_p += p;
            while (x % p == 0)
            {
                x /= p;
                ++e;
            }
            sum_e += e;
        }
    }

    /* If a prime > sqrt(x) remains. */
    if (x > 1)
    {
        sum_p += x;
        sum_e += 1;
    }

    return (seq_t)(sum_p + sum_e);
}

seq_t a(idx_t n)
{
    /* For n=5 the starting value already equals 5. */
    if ((seq_t)n == 5)
        return (seq_t)1;

    seq_t cur = (seq_t)n;
    idx_t steps = 0;

    /* Apply b repeatedly until the value 5 is reached. */
    while (cur != (seq_t)5)
    {
        cur = b_func(cur);
        ++steps;
        /* Safeguard: if the chain grows too long we bail out. */
        if (steps > 1000000)
            break; /* should not happen for the intended range */
    }

    return (seq_t)(steps + 1);
}
