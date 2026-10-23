/*
 * A018730  Divisors of 935.
 * https://oeis.org/A018730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 935 in ascending order */
seq_t a(idx_t n)
{
    static seq_t divisors[8];
    static int ready = 0;

    if (!ready)
    {
        /* 935 = 5 · 11 · 17 */
        static const seq_t primes[3] = {(seq_t)5,(seq_t)11,(seq_t)17};

        unsigned int mask;
        unsigned int idx = 0;
        for (mask = 0; mask < 8; ++mask)        /* 3 primes → 2³ combinations */
        {
            seq_t val = 1;
            unsigned int bit;
            for (bit = 0; bit < 3; ++bit)
                if (mask & (1 << bit))
                    val = val * primes[bit];
            divisors[idx++] = val;
        }

        /* insertion sort – 8 elements is trivial */
        unsigned int i, j;
        for (i = 1; i < 8; ++i)
        {
            seq_t key = divisors[i];
            j = i;
            while (j > 0 && divisors[j - 1] > key)
            {
                divisors[j] = divisors[j - 1];
                --j;
            }
            divisors[j] = key;
        }

        ready = 1;
    }

    if (n < 1 || n > 8)
        return (seq_t)0;
    return divisors[(unsigned int)(n - 1)];
}
