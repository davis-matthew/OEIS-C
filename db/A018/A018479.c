/*
 * A018479  Divisors of 483.
 * https://oeis.org/A018479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 483 (sorted ascending) */

seq_t a(idx_t n)
{
    /* Prime factorization of 483 is 3 * 7 * 23. */
    static const seq_t primes[3] = {(seq_t)3, (seq_t)7, (seq_t)23};

    static seq_t divisors[8] = {0};
    static idx_t initialized = 0;

    if (!initialized)
    {
        /* Generate all divisors via binary masks of the 3 primes. */
        for (idx_t mask = 0; mask < 8; ++mask)
        {
            seq_t prod = 1;
            for (idx_t i = 0; i < 3; ++i)
                if (mask & ((idx_t)1 << i))
                    prod *= primes[i];
            divisors[mask] = prod;
        }

        /* Sort the 8 elements in ascending order (simple bubble sort). */
        for (idx_t i = 0; i < 8; ++i)
            for (idx_t j = i + 1; j < 8; ++j)
                if (divisors[i] > divisors[j])
                {
                    seq_t tmp = divisors[i];
                    divisors[i] = divisors[j];
                    divisors[j] = tmp;
                }

        initialized = 1;
    }

    return (n >= 1 && n <= 8) ? divisors[n - 1] : (seq_t)0;
}
