/*
 * A018652  Divisors of 798.
 * https://oeis.org/A018652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = n-th divisor of 798 in ascending order
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool initialized = false;
    static const seq_t primes[4] = {2, 3, 7, 19};
    static seq_t divisors[16];
    static idx_t count = 0;

    if (!initialized) {
        count = 0;
        for (idx_t mask = 0; mask < 16; ++mask) {
            seq_t val = 1;
            for (int i = 0; i < 4; ++i) {
                if (mask & (idx_t)(1 << i))
                    val *= primes[i];
            }
            divisors[count++] = val;
        }
        /* Insertion sort */
        for (idx_t i = 1; i < count; ++i) {
            seq_t tmp = divisors[i];
            idx_t j = i;
            while (j > 0 && divisors[j-1] > tmp) {
                divisors[j] = divisors[j-1];
                --j;
            }
            divisors[j] = tmp;
        }
        initialized = true;
    }

    if (n == 0) return 0;           /* safety for off‑by‑one callers */
    if (n <= count) return divisors[n-1];
    return 0;                         /* undefined for n > 16 */
}
