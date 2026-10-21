/*
 * A062763  a(n) is the greatest common divisor of (n-1)! and n^n.
 * https://oeis.org/A062763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd((n-1)!, n^n) = product_{p|n} p^{v_p((n-1)!)}
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 23, u64 n <= 41, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utility to compute p^e for seq_t using repeated multiplication.
   Works with any seq_t that supports the ·operator. */
static seq_t seq_pow(seq_t base, idx_t exp)
{
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i)
        res *= base;
    return res;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;         /* a(1) = 1 */

    /* Factor n into primes.  We use a small trial division
       routine; n is at most 64‑bit and the call is fast enough
       for the required range. */
    uint64_t primes[32];
    int exps[32];
    int cnt = 0;
    uint64_t temp = n;
    for (uint64_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            primes[cnt] = p;
            exps[cnt] = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++exps[cnt];
            }
            ++cnt;
        }
    }
    if (temp > 1)
    {
        primes[cnt] = temp;
        exps[cnt] = 1;
        ++cnt;
    }

    seq_t result = 1;
    idx_t m = n - 1;

    for (int i = 0; i < cnt; ++i)
    {
        idx_t p = primes[i];
        /* compute exponent of p in (n-1)! via Legendre's formula */
        idx_t exp = 0;
        idx_t div = p;
        while (div <= m)
        {
            exp += m / div;
            if (div > m / p)   /* prevent overflow */
                break;
            div *= p;
        }
        if (exp == 0)
            continue;          /* prime larger than n-1, but this won't happen */
        seq_t p_pow = seq_pow((seq_t)p, exp);
        result *= p_pow;
    }
    return result;
}
