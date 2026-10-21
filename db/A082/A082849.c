/*
 * A082849  Product of cototient values of consecutive integers.
 * https://oeis.org/A082849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n - φ(n)) * (n+1 - φ(n+1)) (φ = Euler totient)
 * Range:     u8 n <= 34, u16 n <= 493, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t phi_of(idx_t value)
{
    seq_t n = (seq_t)value;
    seq_t result = n;
    seq_t temp = n;
    for (seq_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            while (temp % p == 0)
                temp /= p;
            result = result / p * (p - 1);
        }
    }
    if (temp > 1)
        result = result / temp * (temp - 1);
    return result;
}

seq_t a(idx_t n)
{
    seq_t orig_n = (seq_t)n;
    seq_t n1 = orig_n + 1;

    seq_t phi_n   = phi_of(n);
    seq_t phi_n1  = phi_of(n + 1);

    seq_t cot_n   = orig_n - phi_n;
    seq_t cot_n1  = n1 - phi_n1;

    return cot_n * cot_n1;
}
