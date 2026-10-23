/*
 * A018276  Divisors of 84.
 * https://oeis.org/A018276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 84. a(n) = the nth smallest divisor of 84; a(n)=0 for n>12. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t divs[12];
    static bool init = false;
    if (!init) {
        int idx = 0;
        for (int e2 = 0; e2 <= 2; ++e2) {
            seq_t val2 = 1;
            for (int i = 0; i < e2; ++i) val2 *= 2;
            for (int e3 = 0; e3 <= 1; ++e3) {
                seq_t val3 = val2;
                for (int i = 0; i < e3; ++i) val3 *= 3;
                for (int e7 = 0; e7 <= 1; ++e7) {
                    seq_t val4 = val3;
                    for (int i = 0; i < e7; ++i) val4 *= 7;
                    divs[idx++] = val4;
                }
            }
        }
        /* In‑place insertion sort */
        for (int i = 1; i < 12; ++i) {
            seq_t key = divs[i];
            int j = i - 1;
            while (j >= 0 && divs[j] > key) {
                divs[j + 1] = divs[j];
                --j;
            }
            divs[j + 1] = key;
        }
        init = true;
    }

    if (n < 1) return 0;
    if (n <= 12) return divs[(unsigned)(n - 1)];
    return 0;
}
