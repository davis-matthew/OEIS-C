/*
 * A062796  Inverse Moebius transform of f(n) = n^n (A000312).
 * https://oeis.org/A062796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^d
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 379 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the power d^d for a divisor d.
 * The exponentiation is performed by exponentiation by squaring,
 * which is efficient even for large exponents when seq_t is an
 * arbitrary‑precision integer type. */
static seq_t pow_self(idx_t d)
{
    seq_t result = 1;
    seq_t base   = d;          /* convert to seq_t */
    idx_t exp    = d;

    while (exp > 0)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }
    return result;
}

/* Main sequence function.  It enumerates all divisors of n and
 * accumulates the sum of each divisor raised to itself. */
seq_t a(idx_t n)
{
    seq_t sum = 0;

    for (idx_t i = 1; i <= n / i; ++i)   /* i runs over all divisors <= sqrt(n) */
    {
        if ((n % i) == 0)
        {
            sum += pow_self(i);           /* i^i  */
            idx_t j = n / i;
            if (j != i)
                sum += pow_self(j);       /* (n/i)^(n/i) */
        }
    }

    return sum;
}
