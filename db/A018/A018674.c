/*
 * A018674  Divisors of 836.
 * https://oeis.org/A018674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Divisors of 836, sorted increasingly; a(n) returns the n-th divisor */
    static seq_t divisors[12];
    static bool initialized = false;

    if (!initialized) {
        size_t pos = 0;
        for (idx_t e2 = 0; e2 <= 2; ++e2) {
            seq_t v2 = 1;
            for (idx_t i = 0; i < e2; ++i) v2 *= 2;      /* 2^e2 */
            for (idx_t e11 = 0; e11 <= 1; ++e11) {
                seq_t v11 = v2;
                if (e11) v11 *= 11;                    /* 2^e2 * 11^e11 */
                for (idx_t e19 = 0; e19 <= 1; ++e19) {
                    seq_t v = v11;
                    if (e19) v *= 19;                  /* 2^e2 * 11^e11 * 19^e19 */
                    divisors[pos++] = v;
                }
            }
        }
        /* small fixed-size sort (insertion sort) */
        for (size_t i = 1; i < 12; ++i) {
            seq_t key = divisors[i];
            size_t j = i;
            while (j > 0 && key < divisors[j - 1]) {
                divisors[j] = divisors[j - 1];
                --j;
            }
            divisors[j] = key;
        }
        initialized = true;
    }

    if (n < 1 || n > 12)
        return 0;                                  /* undefined beyond length */
    return divisors[n - 1];
}
