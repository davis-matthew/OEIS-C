/*
 * A027901  Divisors of 10^9 + 1.
 * https://oeis.org/A027901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the nth divisor of 10^9+1 (1000000001) in ascending order
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* 10^9+1 = 7 · 11 · 13 · 19 · 52579 : 5 prime factors → 32 divisors */
    static seq_t divisors[32];
    static bool ready = false;
    static size_t count = 0;

    if (!ready)
    {
        /* Build the list of all divisors */
        const uint64_t primes[5] = {7ULL, 11ULL, 13ULL, 19ULL, 52579ULL};

        for (size_t mask = 0; mask < (1U << 5); ++mask)
        {
            uint64_t prod = 1ULL;
            for (size_t i = 0; i < 5; ++i)
                if (mask & (1U << i))
                    prod *= primes[i];
            divisors[count++] = (seq_t)prod;
        }

        /* Sort the 32 elements (insertion sort, fine for N = 32) */
        for (size_t i = 1; i < count; ++i)
        {
            seq_t key = divisors[i];
            size_t j = i;
            while (j > 0 && divisors[j - 1] > key)
            {
                divisors[j] = divisors[j - 1];
                --j;
            }
            divisors[j] = key;
        }

        ready = true;
    }

    /* Bounds check: a(1) … a(32) are defined */
    if (n == 0 || n > count)
        return (seq_t)0;

    return divisors[n - 1];
}
