/*
 * A018657  Divisors of 806.
 * https://oeis.org/A018657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th divisor of 806 in ascending order
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Divisors of 806 = 2 * 13 * 31.  There are 2^3 = 8 divisors. */
    static seq_t divs[8];
    static int initialized = 0;

    if (!initialized)
    {
        const idx_t primes[3] = {2, 13, 31};
        idx_t count = 0;
        /* Generate all products of subsets of primes. */
        for (idx_t mask = 0; mask < ((idx_t)1 << 3); ++mask)
        {
            seq_t val = 1;
            for (idx_t j = 0; j < 3; ++j)
                if (mask & ((idx_t)1 << j))
                    val *= (seq_t)primes[j];
            divs[count++] = val;
        }

        /* Simple insertion sort for 8 elements. */
        for (idx_t i = 1; i < 8; ++i)
        {
            seq_t key = divs[i];
            idx_t j = i;
            while (j > 0 && divs[j - 1] > key)
            {
                divs[j] = divs[j - 1];
                j--;
            }
            divs[j] = key;
        }

        initialized = 1;
    }

    if (n >= 1 && n <= 8)
        return divs[n - 1];
    return (seq_t)0;
}
