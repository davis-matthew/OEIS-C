/*
 * A018741  Divisors of 954.
 * https://oeis.org/A018741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n‑th smallest divisor of 954 (sorted ascending)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0 || n > 12) return 0;   /* out of bounds */
    static seq_t divisors[12];
    static bool init = false;
    if (!init) {
        /* 954 = 2^1 * 3^2 * 53^1 */
        const idx_t e2max = 1, e3max = 2, e53max = 1;
        idx_t count = 0;
        for (idx_t e2 = 0; e2 <= e2max; ++e2) {
            seq_t val2 = (seq_t)1 << e2;          /* 2^e2 */
            for (idx_t e3 = 0; e3 <= e3max; ++e3) {
                seq_t val3 = 1;
                for (idx_t i = 0; i < e3; ++i) val3 *= 3; /* 3^e3 */
                seq_t v23 = val2 * val3;
                for (idx_t e53 = 0; e53 <= e53max; ++e53) {
                    seq_t val53 = 1;
                    for (idx_t i = 0; i < e53; ++i) val53 *= 53; /* 53^e53 */
                    divisors[count++] = v23 * val53;
                }
            }
        }
        /* Sort the array (12 elements) */
        for (idx_t i = 1; i < count; ++i) {
            seq_t key = divisors[i];
            idx_t j = i;
            while (j > 0 && divisors[j - 1] > key) {
                divisors[j] = divisors[j - 1];
                --j;
            }
            divisors[j] = key;
        }
        init = true;
    }
    return divisors[n - 1];
}
