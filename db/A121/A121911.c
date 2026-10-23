/*
 * A121911  First four terms are decimal digits of 1979. Rest are found by
 *          adding four previous terms modulo 10.
 * https://oeis.org/A121911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1560, u16 n <= 1560, u32 n <= 1560, u64 n <= 1560,
 *            u128 n <= 1560, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=9, a(3)=7, a(4)=9;
 * for n>=5  a(n) = (a(n-1)+a(n-2)+a(n-3)+a(n-4)) % 10
 * Period length: 1560 (a(n+1560)=a(n))
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t period[1560];
    static bool initialized = false;

    if (!initialized) {
        /* initial four terms */
        period[0] = (seq_t)1;
        period[1] = (seq_t)9;
        period[2] = (seq_t)7;
        period[3] = (seq_t)9;
        for (idx_t i = 4; i < 1560; ++i) {
            period[i] = (period[i-1] + period[i-2] + period[i-3] + period[i-4]) % (seq_t)10;
        }
        initialized = true;
    }

    if (n == 0)
        return (seq_t)0;             /* behavior below offset is irrelevant */

    idx_t idx = (n - 1) % (idx_t)1560;
    return period[idx];
}
