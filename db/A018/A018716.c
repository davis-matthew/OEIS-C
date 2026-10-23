/*
 * A018716  Divisors of 909.
 * https://oeis.org/A018716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the nth divisor of 909 in ascending order. If n > 6, a(n)
 *            = 0
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    static seq_t divs[6];
    static bool ready = false;
    if (!ready) {
        /* 909 = 3^2 * 101 */
        seq_t tmp[6];
        int tcnt = 0;
        for (int a = 0; a <= 2; ++a) {
            seq_t va = 1;
            for (int i = 0; i < a; ++i)
                va *= (seq_t)3;
            for (int b = 0; b <= 1; ++b) {
                seq_t vb = va;
                for (int j = 0; j < b; ++j)
                    vb *= (seq_t)101;
                tmp[tcnt++] = vb;
            }
        }
        /* sort the divisors (there are at most 6) */
        for (int i = 1; i < tcnt; ++i) {
            seq_t key = tmp[i];
            int j = i - 1;
            while (j >= 0 && tmp[j] > key) {
                tmp[j + 1] = tmp[j];
                --j;
            }
            tmp[j + 1] = key;
        }
        for (int i = 0; i < tcnt && i < 6; ++i)
            divs[i] = tmp[i];
        ready = true;
    }

    if (n > 6)
        return (seq_t)0;
    return divs[(int)(n - 1)];
}
