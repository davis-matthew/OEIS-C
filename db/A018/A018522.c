/*
 * A018522  Divisors of 567.
 * https://oeis.org/A018522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 567 in ascending order (OEIS A018522) */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t divs[10];
    static bool init = false;

    if (!init) {
        /* Primitive factors: 567 = 3^4 * 7 */
        seq_t pow3[5];
        pow3[0] = (seq_t)1;
        for (int i = 1; i <= 4; ++i) pow3[i] = pow3[i-1] * (seq_t)3;

        seq_t pow7[2];
        pow7[0] = (seq_t)1;
        pow7[1] = (seq_t)7;

        int idx = 0;
        for (int e7 = 0; e7 <= 1; ++e7) {
            for (int e3 = 0; e3 <= 4; ++e3) {
                divs[idx++] = pow3[e3] * pow7[e7];
            }
        }

        /* sort ascending */
        for (int i = 0; i < 9; ++i) {
            for (int j = i + 1; j < 10; ++j) {
                if (divs[i] > divs[j]) {
                    seq_t temp = divs[i];
                    divs[i] = divs[j];
                    divs[j] = temp;
                }
            }
        }
        init = true;
    }

    if (n >= 1 && n <= 10) {
        return divs[(idx_t)(n - 1)];
    } else {
        return (seq_t)0;
    }
}
