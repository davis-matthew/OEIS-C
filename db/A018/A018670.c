/*
 * A018670  Divisors of 830.
 * https://oeis.org/A018670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A018670: Divisors of 830 in increasing order. */

seq_t a(idx_t n)
{
    /* Divisors of 830 are 2 * 5 * 83.  We generate them
       once, sort them, and then return the (n-1)-th element
       if it exists.  For n > 8 the sequence is defined as 0. */
    static seq_t divisors[8];
    static int initialized = 0;
    if (!initialized)
    {
        const seq_t primes[3] = {2, 5, 83};
        int k = 0;
        for (int e2 = 0; e2 <= 1; ++e2)
        {
            seq_t prod2 = 1;
            for (int i = 0; i < e2; ++i) prod2 *= primes[0];
            for (int e5 = 0; e5 <= 1; ++e5)
            {
                seq_t prod5 = prod2;
                for (int i = 0; i < e5; ++i) prod5 *= primes[1];
                for (int e83 = 0; e83 <= 1; ++e83)
                {
                    seq_t prod = prod5;
                    for (int i = 0; i < e83; ++i) prod *= primes[2];
                    divisors[k++] = prod;
                }
            }
        }
        /* Simple insertion sort for the eight numbers */
        for (int i = 0; i < 7; ++i)
        {
            for (int j = i + 1; j < 8; ++j)
            {
                if (divisors[j] < divisors[i])
                {
                    seq_t tmp = divisors[i];
                    divisors[i] = divisors[j];
                    divisors[j] = tmp;
                }
            }
        }
        initialized = 1;
    }

    if (n == 0 || n > 8) return 0;
    return divisors[n - 1];
}
