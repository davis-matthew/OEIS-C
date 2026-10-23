/*
 * A027454  First diagonal of A027448.
 * https://oeis.org/A027454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (LCM(1..n) / n)^3
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Helper for computing small powers in seq_t */
static seq_t pow_seq_small(seq_t base, idx_t exp)
{
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i)
        result *= base;
    return result;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* offset starts at 1 */

    seq_t prod = 1;
    const idx_t orig_n = n;

    /* Enumerate primes up to n */
    for (idx_t p = 2; p <= n; ++p)
    {
        /* simple primality test */
        bool is_prime = true;
        for (idx_t d = 2; d <= p / d; ++d)
        {
            if (p % d == 0)
            {
                is_prime = false;
                break;
            }
        }
        if (!is_prime)
            continue;

        /* e1 = floor(log_p n) */
        idx_t e1 = 0;
        idx_t pow_v = 1;
        while (pow_v <= n / p)
        {
            pow_v *= p;
            ++e1;
        }

        /* v_p(n) : exponent of p in n */
        idx_t vp = 0;
        idx_t tmp = orig_n;
        while (tmp % p == 0)
        {
            ++vp;
            tmp /= p;
        }

        idx_t exp = 0;
        if (e1 > vp)
            exp = e1 - vp;

        if (exp > 0)
            prod *= pow_seq_small((seq_t)p, exp);
    }

    /* result = prod^3 */
    seq_t res = prod;
    res *= prod;   /* prod^2 */
    res *= prod;   /* prod^3 */

    return res;
}
