/*
 * A018550  Divisors of 612.
 * https://oeis.org/A018550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n‑th divisor of 612, listed in ascending order (OEIS
 *            A018550)
 * Range:     u8 n <= 11, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* 612 = 2^2 * 3^2 * 17^1  → 18 divisors */
    static seq_t cache[18];
    static int initialized = 0;

    if (!initialized) {
        /* generate all divisors in any order */
        int pos = 0;
        for (int e2 = 0; e2 <= 2; ++e2) {
            seq_t v2 = 1;
            for (int i = 0; i < e2; ++i) v2 *= 2;
            for (int e3 = 0; e3 <= 2; ++e3) {
                seq_t v3 = v2;
                for (int j = 0; j < e3; ++j) v3 *= 3;
                for (int e17 = 0; e17 <= 1; ++e17) {
                    seq_t v = v3;
                    if (e17) v *= 17;
                    cache[pos++] = v;
                }
            }
        }

        /* simple insertion sort – works for both integral and user‑defined integer types */
        for (size_t i = 1; i < 18; ++i) {
            seq_t key = cache[i];
            size_t j = i;
            while (j > 0 && cache[j - 1] > key) {
                cache[j] = cache[j - 1];
                --j;
            }
            cache[j] = key;
        }

        initialized = 1;
    }

    if (n < 1 || n > 18) return (seq_t)0;
    return cache[(size_t)(n - 1)];
}
