/*
 * A163106  a(n) = sigma(sigma(tau(n))), where tau = number of divisors and
 *          sigma = sum of divisors.
 * https://oeis.org/A163106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(sigma(tau(n))) where * tau(n) = number of divisors
 *            of n, * sigma(m) = sum of divisors of m
 * Range:     u8 n <= 5039, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 105 terms through n = 9227 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute sum of divisors of an unsigned integer m. */
static seq_t _sigma_u64(idx_t m)
{
    seq_t sum = 0;
    for (idx_t d = 1; d <= m / d; ++d)
    {
        if (m % d == 0)
        {
            sum += (seq_t)d;
            idx_t twin = m / d;
            if (twin != d)
                sum += (seq_t)twin;
        }
    }
    return sum;
}
/* Compute number of divisors of an unsigned integer n. */
static seq_t _tau_u64(idx_t n)
{
    seq_t tau = 1;
    idx_t temp = n;
    for (idx_t p = 2; p <= temp / p; ++p)
    {
        if (temp % p == 0)
        {
            idx_t e = 0;
            while (temp % p == 0)
            {
                ++e;
                temp /= p;
            }
            tau *= (seq_t)(e + 1);
        }
    }
    if (temp > 1)          /* remaining prime factor */
        tau *= 2;
    return tau;
}

seq_t a(idx_t n)
{
    seq_t tau_val = _tau_u64(n);          /* tau(n) fits in idx_t for n ≤ 2^64-1 */
    idx_t tau = (idx_t)tau_val;
    seq_t first_sigma = _sigma_u64(tau);
    idx_t first_sigma_idx = (idx_t)first_sigma;
    seq_t result = _sigma_u64(first_sigma_idx);
    return result;
}
