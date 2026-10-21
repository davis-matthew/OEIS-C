/*
 * A086148  Sum of the orders of the elements in the dihedral group D_2n.
 * https://oeis.org/A086148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n + Sum_{d|n} d*phi(d)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t phi_uint64(idx_t m)
{
    seq_t result = (seq_t)m;
    idx_t t = m;
    for (idx_t p = 2; p * p <= t; ++p)
    {
        if (t % p == 0)
        {
            while (t % p == 0)
                t /= p;
            result -= result / (seq_t)p;
        }
    }
    if (t > 1)
        result -= result / (seq_t)t;
    return result;
}

seq_t a(idx_t n)
{
    seq_t total = (seq_t)2 * (seq_t)n;
    seq_t sum   = 0;
    for (idx_t d = 1; d <= n / d; ++d)
    {
        if (n % d == 0)
        {
            seq_t phi_d = phi_uint64(d);
            sum += (seq_t)d * phi_d;
            idx_t d2 = n / d;
            if (d2 != d)
            {
                seq_t phi_d2 = phi_uint64(d2);
                sum += (seq_t)d2 * phi_d2;
            }
        }
    }
    return total + sum;
}
