/*
 * A206350  Position of 1/n in the canonical bijection from the positive
 *          integers to the positive rational numbers.
 * https://oeis.org/A206350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * Σ_{k=1}^{n-1} φ(k), a(1) = 1 where φ(k) is Euler's
 *            totient function
 * Range:     u8 n <= 20, u16 n <= 329, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t
phi_of(idx_t x)
{
    if (x <= 1)
        return (seq_t)1;
    seq_t result = (seq_t)x;
    idx_t p;
    for (p = 2; p * p <= x; ++p)
    {
        if (x % p == 0)
        {
            while (x % p == 0)
                x /= p;
            result -= result / (seq_t)p;
        }
    }
    if (x > 1)
        result -= result / (seq_t)x;
    return result;
}

seq_t
a(idx_t n)
{
    if (n == 0)            /* undefined in OEIS, return 0 */
        return (seq_t)0;
    if (n == 1)            /* first term */
        return (seq_t)1;
    seq_t sum = (seq_t)0;
    idx_t k;
    for (k = 1; k < n; ++k)
        sum += phi_of(k);
    return (seq_t)2 * sum;
}
