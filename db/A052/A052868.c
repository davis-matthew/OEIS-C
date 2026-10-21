/*
 * A052868  Expansion of e.g.f. LambertW(x/(-1+x))/x*(-1+x).
 * https://oeis.org/A052868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..n} n!/k! * C(n-1, k-1) * (k+1)^(k-1) (a(0)=1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)1;

    /* factorial_part = n! / 1! = n!  (will be updated for each k) */
    seq_t factorial_part = 1;
    for (idx_t i = 2; i <= n; ++i)
        factorial_part *= (seq_t)i;

    /* ck   = C(n-1, k-1);  start with k=1 → C(n-1,0) = 1   */
    seq_t ck = 1;

    seq_t sum = 0;

    for (idx_t k = 1; k <= n; ++k)
    {
        /* compute pk = (k+1)^(k-1) using exponentiation by squaring */
        seq_t pk = 1;
        if (k > 1)
        {
            seq_t base = (seq_t)(k + 1);
            idx_t exp  = k - 1;
            while (exp > 0)
            {
                if (exp & 1)
                    pk *= base;
                base *= base;
                exp >>= 1;
            }
        }

        /* term_k = factorial_part * ck * pk */
        seq_t term = factorial_part * ck;
        term = term * pk;

        sum += term;

        if (k == n)
            break;

        /* prepare factorial_part and ck for the next k+1 */
        factorial_part /= (seq_t)(k + 1);
        ck *= (seq_t)(n - k);
        ck /= (seq_t)k;
    }

    return sum;
}
