/*
 * A018437  Divisors of 406.
 * https://oeis.org/A018437
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

/* Divisors of 406 (sorted ascending) */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t divisors[8];
    static bool init = false;

    if (!init) {
        seq_t primes[3];
        primes[0] = (seq_t)2;
        primes[1] = (seq_t)7;
        primes[2] = (seq_t)29;

        int count = 0;
        for (int i = 0; i <= 1; ++i)
            for (int j = 0; j <= 1; ++j)
                for (int k = 0; k <= 1; ++k) {
                    seq_t val = 1;
                    if (i) val *= primes[0];
                    if (j) val *= primes[1];
                    if (k) val *= primes[2];
                    divisors[count++] = val;
                }

        /* insertion sort for the 8 elements */
        for (int i = 1; i < 8; ++i) {
            seq_t key = divisors[i];
            int j = i - 1;
            while (j >= 0 && divisors[j] > key) {
                divisors[j + 1] = divisors[j];
                --j;
            }
            divisors[j + 1] = key;
        }
        init = true;
    }

    if (n < 1 || n > 8)
        return 0;           /* out of range */
    return divisors[(size_t)n - 1];
}
