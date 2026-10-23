/*
 * A018326  Divisors of 189.
 * https://oeis.org/A018326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 189 (sorted ascending), a(n) = 0 for n>8
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;                         /* undefined index, safe return */
    static seq_t divisors[8];
    static bool initialized = false;

    if (!initialized) {
        /* Compute powers of 3: 3^0 .. 3^3 */
        seq_t pow3[4];
        pow3[0] = (seq_t)1;
        for (int i = 1; i <= 3; ++i)
            pow3[i] = pow3[i - 1] * (seq_t)3;

        /* Generate all 8 divisors 3^e3 * 7^e7 */
        int pos = 0;
        for (int e3 = 0; e3 <= 3; ++e3) {
            seq_t v3 = pow3[e3];
            for (int e7 = 0; e7 <= 1; ++e7) {
                seq_t val = v3 * (e7 ? (seq_t)7 : (seq_t)1);
                divisors[pos++] = val;
            }
        }

        /* Sort the 8 values ascending (simple bubble sort) */
        for (int i = 0; i < 8; ++i)
            for (int j = i + 1; j < 8; ++j)
                if (divisors[j] < divisors[i]) {
                    seq_t tmp = divisors[i];
                    divisors[i] = divisors[j];
                    divisors[j] = tmp;
                }

        initialized = true;
    }

    if (n <= 8)
        return divisors[(uint64_t)(n - 1)];

    return (seq_t)0;  /* undefined part of the sequence */
}
