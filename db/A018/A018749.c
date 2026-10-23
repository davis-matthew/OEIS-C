/*
 * A018749  Divisors of 968.
 * https://oeis.org/A018749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 968 in ascending order
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* number of divisors of 968 = 12 */
    static seq_t divisors[12];
    static size_t dlen = 0;

    if (dlen == 0) {
        /* generate all divisors via prime exponents: 2^e1 * 11^e2   */
        seq_t vals[12];
        size_t idx = 0;
        for (int e1 = 0; e1 <= 3; ++e1) {
            seq_t v1 = 1;
            for (int i = 0; i < e1; ++i) v1 *= 2U;
            for (int e2 = 0; e2 <= 2; ++e2) {
                seq_t v2 = v1;
                for (int j = 0; j < e2; ++j) v2 *= 11U;
                vals[idx++] = v2;
            }
        }
        /* sort the 12 values ascending (simple bubble sort) */
        for (size_t i = 0; i < 12; ++i) {
            for (size_t j = i + 1; j < 12; ++j) {
                if (vals[j] < vals[i]) {
                    seq_t t = vals[i];
                    vals[i] = vals[j];
                    vals[j] = t;
                }
            }
        }
        for (size_t i = 0; i < 12; ++i) divisors[i] = vals[i];
        dlen = 12;
    }

    if (n == 0 || n > dlen) return 0;   /* undefined beyond the finite list */
    return divisors[(size_t)(n - 1)];
}
