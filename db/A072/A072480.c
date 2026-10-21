/*
 * A072480  Shadow transform of factorials A000142.
 * https://oeis.org/A072480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - smallest m such that n | m! (for n>1) For n = 0, a(0)
 *            = 0; for n = 1, a(1) = 1
 * Range:     u8 n <= 269, u16 n <= 9827, u32 n <= 9827, u64 n <= 9827,
 *            u128 n <= 9827, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* compute exponent of prime p in m! */
static unsigned long long vp_fact(unsigned long long m, unsigned long long p)
{
    unsigned long long exp = 0;
    while (m)
    {
        m /= p;
        exp += m;
    }
    return exp;
}

/* minimal m such that vp_fact(m,p) >= e */
static unsigned long long min_m_for_prime_power(unsigned long long p, unsigned long long e, unsigned long long n)
{
    unsigned long long lo = 0, hi = n;
    while (lo < hi)
    {
        unsigned long long mid = (lo + hi) >> 1;
        if (vp_fact(mid, p) >= e)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    unsigned long long N = n;
    unsigned long long m_needed = 0;               /* minimal m that works for all primes */

    unsigned long long tmp = N;
    for (unsigned long long p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            unsigned long long e = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++e;
            }
            unsigned long long m_p = min_m_for_prime_power(p, e, N);
            if (m_p > m_needed)
                m_needed = m_p;
        }
    }
    if (tmp > 1) /* remaining prime factor */
    {
        unsigned long long p = tmp;
        unsigned long long e = 1;
        unsigned long long m_p = min_m_for_prime_power(p, e, N);
        if (m_p > m_needed)
            m_needed = m_p;
    }

    return (seq_t)n - (seq_t)m_needed;
}
