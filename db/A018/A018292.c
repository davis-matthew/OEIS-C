/*
 * A018292  Divisors of 117.
 * https://oeis.org/A018292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the nth divisor of 117 (sorted ascending). a(n)=0 for n>6
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t divisors[6];
    static bool initialized = false;

    if (!initialized) {
        /* 117 = 3^2 * 13 */
        const idx_t p1 = 3, e1 = 2;
        const idx_t p2 = 13, e2 = 1;

        int pos = 0;
        for (idx_t exp1 = 0; exp1 <= e1; ++exp1) {
            seq_t val1 = 1;
            for (idx_t i = 0; i < exp1; ++i)
                val1 *= (seq_t)p1;
            for (idx_t exp2 = 0; exp2 <= e2; ++exp2) {
                seq_t val = val1;
                for (idx_t i = 0; i < exp2; ++i)
                    val *= (seq_t)p2;
                divisors[pos++] = val;
            }
        }

        /* simple insertion sort to order the divisors ascending */
        for (int i = 1; i < 6; ++i) {
            seq_t key = divisors[i];
            int j = i - 1;
            while (j >= 0 && divisors[j] > key) {
                divisors[j + 1] = divisors[j];
                j--;
            }
            divisors[j + 1] = key;
        }

        initialized = true;
    }

    if (n == 0 || n > 6)
        return (seq_t)0;
    return divisors[(int)(n - 1)];
}
