/*
 * A058005  a(n) = gcd(2*n, binomial(2*n, n)).
 * https://oeis.org/A058005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = GCD(2*n, binomial(2*n,n))
 * Range:     u8 n <= 139, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t vp_fact(idx_t m, idx_t p)
{
    idx_t sum = 0;
    while (m)
    {
        m /= p;
        sum += m;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* Undefined, but safe for completeness */

    /* Factor n */
    idx_t primes[20];
    idx_t exps[20];
    int cnt = 0;

    idx_t temp = n;
    for (idx_t i = 2; i * i <= temp; ++i)
    {
        if (temp % i == 0)
        {
            int e = 0;
            while (temp % i == 0)
            {
                temp /= i;
                ++e;
            }
            primes[cnt] = i;
            exps[cnt] = e;
            ++cnt;
        }
    }
    if (temp > 1)
    {
        primes[cnt] = temp;
        exps[cnt] = 1;
        ++cnt;
    }

    /* Add the extra factor 2 from 2*n */
    int idx2 = -1;
    for (int j = 0; j < cnt; ++j)
    {
        if (primes[j] == 2)
        {
            idx2 = j;
            break;
        }
    }
    if (idx2 == -1)
    {
        primes[cnt] = 2;
        exps[cnt] = 1;
        ++cnt;
    }
    else
    {
        ++exps[idx2]; /* one more 2 */
    }

    seq_t result = 1;
    idx_t n2 = n * 2;

    for (int j = 0; j < cnt; ++j)
    {
        idx_t p = primes[j];
        int exp2n = exps[j];

        idx_t vp_bin = vp_fact(n2, p) - 2 * vp_fact(n, p);
        int exp = (vp_bin < exp2n) ? vp_bin : exp2n;

        seq_t power = 1;
        for (int k = 0; k < exp; ++k)
            power *= p;

        result *= power;
    }

    return result;
}
