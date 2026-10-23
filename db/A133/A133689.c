/*
 * A133689  a(n) = smallest integer that is > n and is a multiple of every
 *          proper divisor of n.
 * https://oeis.org/A133689
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m > n such that m is a multiple of all proper
 *            divisors of n
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    uint64_t nn = n;           /* local copy as 64‑bit integer */
    uint64_t temp = nn;
    uint64_t first = 0;        /* first distinct prime factor */
    int multiple = 0;          /* flag: more than one distinct prime factor */

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            if (first == 0)
                first = p;
            else if (first != p)
                multiple = 1;
            while (temp % p == 0)
                temp /= p;
        }
    }
    if (temp > 1) {           /* remaining prime factor > sqrt(original n) */
        if (first == 0)
            first = temp;
        else if (first != temp)
            multiple = 1;
    }

    seq_t L;
    if (multiple)
        L = (seq_t)nn;              /* ≥2 distinct primes → lcm = n */
    else
        L = (seq_t)(nn / first);     /* prime power → lcm = n / prime */

    seq_t div = (seq_t)nn / L;        /* floor(n / L) */
    seq_t result = (div + (seq_t)1) * L; /* smallest multiple of L > n */

    return result;
}
