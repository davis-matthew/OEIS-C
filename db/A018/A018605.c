/*
 * A018605  Divisors of 712.
 * https://oeis.org/A018605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th divisor of 712 in increasing order (0 for n>8)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    static seq_t sorted_divs[8];
    static int initialized = 0;
    if (!initialized) {
        seq_t pow2[4];
        pow2[0] = 1;
        for (int i = 1; i < 4; ++i) pow2[i] = pow2[i - 1] * 2;
        int cnt = 0;
        for (int e2 = 0; e2 < 4; ++e2) {
            for (int e89 = 0; e89 < 2; ++e89) {
                seq_t val = pow2[e2] * (e89 ? (seq_t)89 : (seq_t)1);
                sorted_divs[cnt++] = val;
            }
        }
        /* insertion sort */
        for (int i = 1; i < 8; ++i) {
            seq_t key = sorted_divs[i];
            int j = i - 1;
            while (j >= 0 && sorted_divs[j] > key) {
                sorted_divs[j + 1] = sorted_divs[j];
                --j;
            }
            sorted_divs[j + 1] = key;
        }
        initialized = 1;
    }
    if (n <= 8) return sorted_divs[(idx_t)n - 1];
    return 0;
}
