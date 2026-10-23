/*
 * A018470  Divisors of 465.
 * https://oeis.org/A018470
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

/* Sequence: Divisors of 465.  Returns the n‑th divisor (ascending order);
   n > 8 is undefined (returns 0). */
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t divisors[8];
    static int init = 0;

    if (!init) {
        const seq_t primes[3] = {3, 5, 31};
        for (int mask = 0; mask < 8; ++mask) {
            seq_t val = 1;
            if (mask & 1) val *= primes[0];
            if (mask & 2) val *= primes[1];
            if (mask & 4) val *= primes[2];
            divisors[mask] = val;
        }
        /* simple insertion sort */
        for (int i = 1; i < 8; ++i) {
            seq_t key = divisors[i];
            int j = i - 1;
            while (j >= 0 && divisors[j] > key) {
                divisors[j + 1] = divisors[j];
                --j;
            }
            divisors[j + 1] = key;
        }
        init = 1;
    }

    if (n >= 1 && n <= 8)
        return divisors[(size_t)(n - 1)];
    else
        return 0;
}
