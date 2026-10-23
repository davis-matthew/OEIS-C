/*
 * A018568  Divisors of 645.
 * https://oeis.org/A018568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 645 (sorted increasingly)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Constant 645 = 3 * 5 * 43 */
    static seq_t divisors[8];
    static bool initialized = false;

    if (!initialized) {
        const uint64_t primes[3] = {3ULL, 5ULL, 43ULL};
        const int exps[3] = {1, 1, 1};
        const int num = 1 << 3;            /* 8 divisors */

        for (int mask = 0; mask < num; ++mask) {
            seq_t value = 1;
            for (int p = 0; p < 3; ++p) {
                if ((mask >> p) & 1)      /* exponent 1 if bit set */
                    value *= (seq_t)primes[p];
            }
            divisors[mask] = value;
        }

        /* simple insertion sort for 8 elements */
        for (int i = 1; i < num; ++i) {
            seq_t key = divisors[i];
            int j = i - 1;
            while (j >= 0 && divisors[j] > key) {
                divisors[j + 1] = divisors[j];
                --j;
            }
            divisors[j + 1] = key;
        }
        initialized = true;
    }

    /* Guard against out‑of‑range indices */
    if (n < 1 || n > (idx_t) (sizeof(divisors) / sizeof(divisors[0])))
        return 0;

    return divisors[(size_t)n - 1];
}
