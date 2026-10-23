/*
 * A018264  Divisors of 54.
 * https://oeis.org/A018264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the nth divisor of 54 in ascending order
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t divisors[8];
static bool init = false;

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (!init) {
        int idx = 0;
        /* generate all divisors of 54 = 2^1 * 3^3 */
        for (int a = 0; a <= 1; ++a) {
            seq_t pow2 = 1;
            for (int i = 0; i < a; ++i) pow2 *= (seq_t)2;
            for (int b = 0; b <= 3; ++b) {
                seq_t pow3 = 1;
                for (int i = 0; i < b; ++i) pow3 *= (seq_t)3;
                divisors[idx++] = pow2 * pow3;
            }
        }
        /* simple insertion sort: 8 elements only */
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
    if (n <= 8) return divisors[n - 1];
    return (seq_t)0;
}
