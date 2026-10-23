/*
 * A018270  Divisors of 70.
 * https://oeis.org/A018270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   Divisors of 70 (finite list)
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static seq_t divisors[8];
    static int initialized = 0;
    if (!initialized)
    {
        seq_t primes[3] = { (seq_t)2, (seq_t)5, (seq_t)7 };
        int idx = 0;
        for (int e0 = 0; e0 <= 1; ++e0)
        {
            seq_t v0 = e0 ? primes[0] : (seq_t)1;
            for (int e1 = 0; e1 <= 1; ++e1)
            {
                seq_t v1 = v0 * (e1 ? primes[1] : (seq_t)1);
                for (int e2 = 0; e2 <= 1; ++e2)
                {
                    divisors[idx++] = v1 * (e2 ? primes[2] : (seq_t)1);
                }
            }
        }
        /* sort ascending */
        for (int i = 0; i < 8; ++i)
            for (int j = i + 1; j < 8; ++j)
                if (divisors[j] < divisors[i])
                {
                    seq_t tmp = divisors[i];
                    divisors[i] = divisors[j];
                    divisors[j] = tmp;
                }
        initialized = 1;
    }
    if (n >= 1 && n <= 8)
        return divisors[n - 1];
    return (seq_t)0;
}
